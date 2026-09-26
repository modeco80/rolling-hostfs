#include "utils/log.hpp"
#include <ml/string.h>

extern "C" {
	extern char ModulePath[64];
	extern char ModuleRebootPackagePath[64];
	extern char MusicPath[64];
	extern char MusicCdSuffix[3];
}

// wad_hooks.cpp
bool wadInitHooks();

extern "C" int modMain() {
	utilLog(LogInfo, "Rolling HostFS patch. 2026 modeco80");

	// hostfs music
	mlStaticStrCpy(MusicPath, "host0:music/%s");
	MusicCdSuffix[0] = '\0';

	// hostfs IOP modules
	mlStaticStrCpy(ModulePath, "host0:modules/%s.irx");
	mlStaticStrCpy(ModuleRebootPackagePath, "host0:modules/ioprp255.img");

	if(!wadInitHooks()) {
		utilLog(LogErr, "Failed to hook WAD manager functions.");
		return 1;
	}


	return 0;
}
