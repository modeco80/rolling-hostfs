#include "utils/log.hpp"
#include "utils/hook/fnhook.hpp"

#include <rolling/wad.h>

FUNC_HOOK(Wad_fopen, void*, const char* path, const char* mode) {
	utilLogf(LogInfo, "Wad_fopen(\"%s\", %s)", path, mode);
	return hook_Wad_fopen.original(path, mode);
}

bool wadInitHooks() {
#define DO_HOOK_FUNC(fnName) \
	if(!hook_##fnName.hook()) { \
		utilLogf(LogErr, "Failed to hook %s.", #fnName); \
		return false; \
	}

	DO_HOOK_FUNC(Wad_fopen);
	return true;
}
