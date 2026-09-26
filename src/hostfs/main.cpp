#include "utils/log.hpp"
#include "utils/hook/fnhook.hpp"

bool wadInitHooks();

extern "C" int modMain() {
	utilLog(LogInfo, "Rolling HostFS patch. 2026 modeco80");

	if(!wadInitHooks()) {
		utilLog(LogErr, "Failed to hook WAD manager functions.");
		return 1;
	}


	return 0;
}
