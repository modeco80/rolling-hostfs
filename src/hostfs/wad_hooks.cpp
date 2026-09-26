#include "utils/log.hpp"
#include "utils/hook/fnhook.hpp"

#include <rolling/wad.h>

FUNC_HOOK(Wad_fopen, void*, const char* path, const char* mode) {
	void* handle = hook_Wad_fopen.original(path, mode);
	utilLogf(LogInfo, "Wad_fopen(\"%s\", %s) -> %08x", path, mode, handle);
	return handle;
}

FUNC_HOOK(Wad_fread, i32, void* pBuffer, i32 size, i32 nitems, void* wadfile) {
	i32 result = hook_Wad_fread.original(pBuffer, size, nitems, wadfile);
	//utilLogf(LogInfo, "Wad_fread(p: 0x%08x, size: %d, nitems: %d, handle: %08x) -> %d", pBuffer, size, nitems, wadfile, result);
	return result;
}

FUNC_HOOK(Wad_fgets, i32, char* pszIn, i32 pszLen, void* handle) {
	i32 result = hook_Wad_fgets.original(pszIn, pszLen, handle);
	utilLogf(LogInfo, "Wad_fgets(%08x) -> \"%s\" (%d)", pszIn, result);
	return result;
}

FUNC_HOOK(Wad_ReadAll, void*, const char* pszFileName) {
	void* result = hook_Wad_ReadAll.original(pszFileName);
	utilLogf(LogInfo, "Wad_ReadAll(\"%s\")", pszFileName);
	return result;
}


FUNC_HOOK(Wad_ReadAllInto, i32, const char* pszFileName, void* pBuffer, i32 count) {
	i32 result = hook_Wad_ReadAllInto.original(pszFileName, pBuffer, count);
	utilLogf(LogInfo, "Wad_ReadAllInto(\"%s\", %d)", pszFileName, count);
	return result;
}

bool wadInitHooks() {
#define DO_HOOK_FUNC(fnName) \
	if(!hook_##fnName.hook()) { \
		utilLogf(LogErr, "Failed to hook %s.", #fnName); \
		return false; \
	}

	DO_HOOK_FUNC(Wad_fopen);
	DO_HOOK_FUNC(Wad_fread);
	DO_HOOK_FUNC(Wad_fgets);
	DO_HOOK_FUNC(Wad_ReadAll);
	DO_HOOK_FUNC(Wad_ReadAllInto);
	return true;
}
