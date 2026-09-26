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

#define FIOMAN_DEBUG

namespace {
	/// Wrapper over EE fio.
	class FioFile {
		i32 fd;
		u32 size;

		void cacheSize() {
			sceLSeek(fd, 0, SCE_SEEK_END);
			size = sceLSeek(fd, 0, SCE_SEEK_CUR);
			sceLSeek(fd, 0, SCE_SEEK_SET);
		}

	public:

		explicit inline FioFile(i32 fd)
		: fd(fd) {
			cacheSize();
		}

		~FioFile() {
			sceClose(fd);
		}

		inline i32 read(void* pvBuf, i32 count) {
			i32 ret =  sceRead(fd, pvBuf, count);;
			//utilLogf(LogInfo, "HostFs Read %d -> %d actually read", count, ret);
			return ret;
		}

		inline i32 tell() {
			return sceLSeek(fd, 0, SCE_SEEK_CUR);
		}

		inline i32 lseek(i32 offset, i32 whence) {
			return sceLSeek(fd, offset, whence);
		}

		inline u32 getSize() const {
			return size;
		}

		inline bool eof() {
			return tell() == size;
		}
	};


	ml::FreeList<FioFile, 8> openFileList;

	void translateFileName(char* pszOut, const char* pszInPath) {
		mlStaticStrCpy(pszOut, "host0:");
		strcat(pszOut, pszInPath);
	}

	FioFile* openFile(const char* path) {
		FioFile* pFile = openFileList.allocate();
		if(pFile == nil(FioFile*))
			return nil(FioFile*);

		char translatedPath[512];
		translateFileName(&translatedPath[0], path);

#ifdef FIOMAN_DEBUG
		utilLogf(LogInfo, "HostFS Open %s", translatedPath);
#endif

		i32 fd = sceOpen(translatedPath, SCE_RDONLY);
		if(fd < 0) {
#ifdef FIOMAN_DEBUG
			utilLogf(LogErr, "HostFS Open FAIL %s", translatedPath);
#endif
			openFileList.free(pFile);
			return nil(FioFile*);
		}

#ifdef FIOMAN_DEBUG
		utilLogf(LogInfo, "HostFS Open SUCCESS %s", translatedPath);
#endif

		// ml freelists don't new objects, so we have to do it ourselves.
		return new (pFile) FioFile(fd);
	}

	void closeFile(FioFile* pFile) {
		pFile->~FioFile();
		openFileList.free(pFile);
	}
}

FUNC_HOOK(Wad_fexist, i32, const char* pszFileName) {
	FioFile* pFile = openFile(pszFileName);
	if(pFile == nil(FioFile*))
		return 0;

	closeFile(pFile);
	return 1;
}

FUNC_HOOK(Wad_fopen, void*, const char* path, const char* mode) {
	if(mode[0] == 'w') {
		utilLogf(LogWarn, "Trying to open %s as read-write. Leaving readonly");
	}

	FioFile* pFile = openFile(path);
	if(pFile == nil(FioFile*)) {
		return vnil;
	}

	return reinterpret_cast<void*>(pFile);
}

FUNC_HOOK(Wad_fclose, void, void* handle) {
	closeFile(reinterpret_cast<FioFile*>(handle));
}

FUNC_HOOK(Wad_feof, i32, void* handle) {
	return reinterpret_cast<FioFile*>(handle)->eof() ? 1 : 0;
}

FUNC_HOOK(Wad_fseek, i32, void* handle, i32 offset, i32 whence) {
	utilLogf(LogInfo, "fseek(%d %d)", offset, whence);
	return reinterpret_cast<FioFile*>(handle)->lseek(offset, whence);
}

FUNC_HOOK(Wad_fread, i32, void* pBuffer, i32 size, i32 nitems, void* wadfile) {
	i32 count = reinterpret_cast<FioFile*>(wadfile)->read(pBuffer, nitems * size);
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
	FioFile* pFile = openFile(pszFileName);
	if(pFile == nil(FioFile*))
		return vnil;

	void* pvBuf = memAllocAligned(pFile->getSize(), 0x80);
	if(pvBuf == vnil)
		return vnil;

	i32 count = pFile->read(pvBuf, pFile->getSize());

	closeFile(pFile);

	Wad_ReadAllFileSize = pFile->getSize();
	Wad_ReadCount++;
	return pvBuf;
}

FUNC_HOOK(Wad_ReadAllInto, i32, const char* pszFileName, void* pBuffer, i32 count) {
	FioFile* pFile = openFile(pszFileName);
	if(pFile == nil(FioFile*))
		return -1;

	if(count == 0) {
		count = pFile->getSize();
#ifdef FIOMAN_DEBUG
		utilLogf(LogInfo, "Wad_ReadAllInto: Count was 0, so reading %d bytes instead", count);
#endif
	}

	i32 countRead = pFile->read(pBuffer, count);

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
