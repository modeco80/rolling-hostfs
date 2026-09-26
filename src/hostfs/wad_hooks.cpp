#include <ml/cxx/freelist.hpp>
#include "utils/log.hpp"
#include "utils/hook/fnhook.hpp"

#include "filemanager.hpp"
#include "file.hpp"

#include <ml/mem.h>
#include <ml/string.h>
#include <rolling/wad.h>
#include <rolling/mem.h>


FUNC_HOOK(Wad_Mount, void, const char* pszWad) {
	return;
}

FUNC_HOOK(Wad_Unmount, void) {
	return;
}

FUNC_HOOK(Wad_fexist, i32, const char* pszFileName) {
	File* pFile = FileMan_openFile(pszFileName);
	if(pFile == nil(File*))
		return 0;

	FileMan_closeFile(pFile);
	return 1;
}

FUNC_HOOK(Wad_fopen, void*, const char* path, const char* mode) {
	if(mode[0] == 'w') {
#ifdef FIOMAN_DEBUG
		utilLogf(LogWarn, "Wad_fopen: Trying to open %s as read-write. Leaving readonly", path);
#endif
	}

	File* pFile = FileMan_openFile(path);
	if(pFile == nil(File*)) {
		return vnil;
	}

	return reinterpret_cast<void*>(pFile);
}

FUNC_HOOK(Wad_fclose, void, void* handle) {
	FileMan_closeFile(reinterpret_cast<File*>(handle));
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
#ifdef FIOMAN_DEBUG
	utilLog(LogWarn, "Wad_fgets: called??");
#endif
	return 0;
}

FUNC_HOOK(Wad_ReadAll, void*, const char* pszFileName) {
	File* pFile = FileMan_openFile(pszFileName);
	if(pFile == nil(File*))
		return vnil;

	void* pvBuf = memAllocAligned(pFile->getSize(), 0x80);
	if(pvBuf == vnil) {
		FileMan_closeFile(pFile);
		return vnil;
	}

	i32 count = pFile->read(reinterpret_cast<u8*>(pvBuf), pFile->getSize());

	FileMan_closeFile(pFile);

	Wad_ReadAllFileSize = pFile->getSize();
	Wad_ReadCount++;
	return pvBuf;
}

FUNC_HOOK(Wad_ReadAllInto, i32, const char* pszFileName, void* pBuffer, i32 count) {
	File* pFile = FileMan_openFile(pszFileName);
	if(pFile == nil(File*))
		return -1;

	if(count == 0) {
		count = pFile->getSize();
#ifdef FIOMAN_DEBUG
		utilLogf(LogInfo, "Wad_ReadAllInto: Count was 0, so reading %d bytes instead", count);
#endif
	}

	i32 countRead = pFile->read(reinterpret_cast<u8*>(pBuffer), count);

	FileMan_closeFile(pFile);

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
