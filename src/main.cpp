#include <ml/string.h>

#include "utils/log.hpp"

// wad_hooks.cpp
bool wadInitHooks();
bool movieInitHooks();

extern "C" int modMain() {
	utilLog(LogInfo, "Rolling HostFS patch. 2026 modeco80");

	if(!wadInitHooks()) {
		utilLog(LogErr, "Failed to hook WAD manager functions.");
		return 1;
	}

	if(!movieInitHooks()) {
		utilLog(LogErr, "Failed to hook movie functions.");
		return 1;
	}

	return 0;
}
