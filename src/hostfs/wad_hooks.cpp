#include "ml/cxx/freelist.hpp"
#include "ml/types.h"
#include "utils/log.hpp"
#include "utils/hook/fnhook.hpp"

#include <new>

#include <ml/mem.h>
#include <ml/string.h>
#include <rolling/wad.h>
#include <rolling/mem.h>
#include <sce/fio.h>

// bit of a hack, but saves needing strcpy directly in cases where
// you only need the literal :)
#define mlStaticStrCpy(dst, srcStrLiteral) memcpy(dst, &(srcStrLiteral)[0], sizeof(srcStrLiteral))

#define mlMin(a, b) ((a) < (b) ? (a) : (b))

#define FIOMAN_DEBUG
#define FIOMAN_DEBUG_OPEN
//#define FIOMAN_DEBUG_READ // verbose as hell
//#define FIOMAN_DEBUG_SEEK

/// the size of the read buffer inside each FioFile instance
/// Try to keep this sensible
#define FIOMAN_READ_BUFFER_SIZE 0x800

namespace {
	/// Wrapper over EE FIO which is a bit easier to use and adds buffering
	class File {
		i32 fd;
		u32 fileSize;

		// Read buffer state
		u32 readBufferPosition;
		u32 readBufferAvailable;
		u32 readBufferStart;
		u8* readBuffer;

		void cacheSize() {
			sceLSeek(fd, 0, SCE_SEEK_END);
			fileSize = sceLSeek(fd, 0, SCE_SEEK_CUR);
			sceLSeek(fd, 0, SCE_SEEK_SET);
		}

	public:

		explicit inline File(i32 fd)
		: fd(fd) {
			cacheSize();

			// Reset buffer state.
			readBufferAvailable = 0;
			readBufferPosition = 0;
			readBufferStart = 0;
			readBuffer = reinterpret_cast<u8*>(mlMalloc(FIOMAN_READ_BUFFER_SIZE));
		}

		~File() {
			mlFree(readBuffer);
			sceClose(fd);
		}

		i32 read(u8* pvBuf, i32 count) {
			i32 total = 0;
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
			utilLogf(LogInfo, "File::read(count: %d)", count);
#endif

			while(count > 0) {
				// Check if the buffer has been used up. If so, then we need to read again.
				if(readBufferPosition == readBufferAvailable) {
					u32 offset = readBufferStart + readBufferPosition;
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
					utilLogf(LogInfo, "File::read() Need to seek to %d to service read buffer", offset);
#endif
					sceLSeek(fd, offset, SCE_SEEK_SET);

					readBufferAvailable = sceRead(fd, &readBuffer[0], FIOMAN_READ_BUFFER_SIZE);

					readBufferStart = offset;
					readBufferPosition = 0;

					if(readBufferAvailable == 0)
						break;
				}

				u32 avail = readBufferAvailable - readBufferPosition;
				u32 n = mlMin(count, avail);

				memcpy(pvBuf, &readBuffer[readBufferPosition], n);

				pvBuf += n;
				readBufferPosition += n;
				count -= n;
				total += n;
			}

#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
			//if(count == 0)
			//	utilLogf(LogInfo, "File::read() read all %d bytes", total);
			//else
			//	utilLogf(LogInfo, "File::read() read %d bytes, %d were not read", total, count);
#endif

			return total;
		}

		i32 tell() {
			return (readBufferStart + readBufferPosition);
		}

		i32 seek(i32 offset, i32 whence) {
			i32 current = readBufferStart + readBufferPosition;
			i32 target;

#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
			utilLogf(LogInfo, "File::seek(offset: %d, whence: %d)", offset, whence);
#endif

			switch(whence) {
				case 0: // relative to begin of file
					target = offset;
					break;
				case 1: // relative to current position
					target = current + offset;
					break;
				case 2: // relative to end of file.
					// If the offset is not negative or 0 then give up
					if(offset != 0 && offset > 0)
						return -1;
					target = fileSize + offset;
					break;
			}

			if(target > fileSize) {
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
				utilLogf(LogErr, "File::seek() Invalid target %d", target);
#endif
				return -1;
			}

			if (target >= readBufferStart && target <= readBufferStart + readBufferAvailable) {
				readBufferPosition = static_cast<i32>(target - readBufferStart);
			} else {
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
				utilLogf(LogInfo, "File::seek() Abandoning buffer");
#endif
				// Discard buffer.
				readBufferStart = target;
				readBufferAvailable = 0;
				readBufferPosition = 0;
				sceLSeek(fd, target, whence);
			}

			return 0;
		}

		u32 getSize() const {
			return fileSize;
		}

		bool eof() {
#ifdef FIOMAN_DEBUG
			utilLogf(LogInfo, "File::eof() ? %s (%d vs %d)", tell() >= fileSize ? "we are at the end": "we are NOT at the end", tell(), fileSize);

			u32 sceTell = sceLSeek(fd, 0, SCE_SEEK_CUR);
			utilLogf(LogInfo, "File::eof() ? What about the underlying fd? %s (%d vs %d)",  sceTell >= fileSize ? "we are at the end": "we are NOT at the end", sceTell, fileSize);
#endif
			return tell() >= fileSize;
		}
	};


	ml::FreeList<File, 8> openFileList;

	void translateFileName(char* pszOut, const char* pszInPath) {
		mlStaticStrCpy(pszOut, "host0:");
		strcat(pszOut, pszInPath);

		// Fix up the objectively incorrect directory seperator to the correct one.
		const u32 len = strlen(pszOut);
		for(u32 i = sizeof("host0:")-1; i < len; ++i)
			if(pszOut[i] == '\\')
				pszOut[i] = '/';
	}

	File* openFile(const char* path) {
		File* pFile = openFileList.allocate();
		if(pFile == nil(File*))
			return nil(File*);

		char translatedPath[512];
		translateFileName(&translatedPath[0], path);

#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_OPEN)
		utilLogf(LogInfo, "HostFS Open %s", translatedPath);
#endif

		i32 fd = sceOpen(translatedPath, SCE_RDONLY);
		if(fd < 0) {
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_OPEN)
			utilLogf(LogErr, "HostFS Open FAIL %s", translatedPath);
#endif
			openFileList.free(pFile);
			return nil(File*);
		}

#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_OPEN)
		utilLogf(LogInfo, "HostFS Open SUCCESS %s", translatedPath);
#endif

		// ml freelists don't new objects, so we have to do it ourselves.
		return new (pFile) File(fd);
	}

	void closeFile(File* pFile) {
		pFile->~File();
		openFileList.free(pFile);
	}
}

FUNC_HOOK(Wad_Mount, void, const char* pszWad) {
	return;
}

FUNC_HOOK(Wad_Unmount, void) {
	return;
}

FUNC_HOOK(Wad_fexist, i32, const char* pszFileName) {
	File* pFile = openFile(pszFileName);
	if(pFile == nil(File*))
		return 0;

	closeFile(pFile);
	return 1;
}

FUNC_HOOK(Wad_fopen, void*, const char* path, const char* mode) {
	if(mode[0] == 'w') {
		utilLogf(LogWarn, "Trying to open %s as read-write. Leaving readonly", path);
	}

	File* pFile = openFile(path);
	if(pFile == nil(File*)) {
		return vnil;
	}

	return reinterpret_cast<void*>(pFile);
}

FUNC_HOOK(Wad_fclose, void, void* handle) {
	closeFile(reinterpret_cast<File*>(handle));
}

FUNC_HOOK(Wad_feof, i32, void* handle) {
	return reinterpret_cast<File*>(handle)->eof() ? 1 : 0;
}

FUNC_HOOK(Wad_fseek, i32, void* handle, i32 offset, i32 whence) {
	return reinterpret_cast<File*>(handle)->seek(offset, whence);
}

FUNC_HOOK(Wad_fread, i32, void* pBuffer, i32 size, i32 nitems, void* wadfile) {
	i32 count = reinterpret_cast<File*>(wadfile)->read(reinterpret_cast<u8*>(pBuffer), nitems * size);
	Wad_ReadCount++;
	return count;
}

FUNC_HOOK(Wad_fgets, i32, char* pszIn, i32 pszLen, void* handle) {
	// For now, since I do not think this is ever actually called,
	// just stub it out.
	utilLog(LogWarn, "Wad_fgets called??");
	return 0;
}

FUNC_HOOK(Wad_ReadAll, void*, const char* pszFileName) {
	File* pFile = openFile(pszFileName);
	if(pFile == nil(File*))
		return vnil;

	void* pvBuf = memAllocAligned(pFile->getSize(), 0x80);
	if(pvBuf == vnil) {
		closeFile(pFile);
		return vnil;
	}

	i32 count = pFile->read(reinterpret_cast<u8*>(pvBuf), pFile->getSize());

	closeFile(pFile);

	Wad_ReadAllFileSize = pFile->getSize();
	Wad_ReadCount++;
	return pvBuf;
}

FUNC_HOOK(Wad_ReadAllInto, i32, const char* pszFileName, void* pBuffer, i32 count) {
	File* pFile = openFile(pszFileName);
	if(pFile == nil(File*))
		return -1;

	if(count == 0) {
		count = pFile->getSize();
#ifdef FIOMAN_DEBUG
		utilLogf(LogInfo, "Wad_ReadAllInto: Count was 0, so reading %d bytes instead", count);
#endif
	}

	i32 countRead = pFile->read(reinterpret_cast<u8*>(pBuffer), count);

	closeFile(pFile);

	Wad_ReadAllFileSize = count;
	Wad_ReadCount++;
	return countRead;
}

bool wadInitHooks() {
#define DO_HOOK_FUNC(fnName) \
	if(!hook_##fnName.hook()) { \
		utilLogf(LogErr, "Failed to hook %s.", #fnName); \
		return false; \
	}

	DO_HOOK_FUNC(Wad_Mount);
	DO_HOOK_FUNC(Wad_Unmount);

	DO_HOOK_FUNC(Wad_fopen);
	DO_HOOK_FUNC(Wad_fclose);
	DO_HOOK_FUNC(Wad_fexist);
	DO_HOOK_FUNC(Wad_feof);
	DO_HOOK_FUNC(Wad_fseek);
	DO_HOOK_FUNC(Wad_fread);
	DO_HOOK_FUNC(Wad_fgets);
	DO_HOOK_FUNC(Wad_ReadAll);
	DO_HOOK_FUNC(Wad_ReadAllInto);
	return true;
}
