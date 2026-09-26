#include "utils/log.hpp"

extern "C" int modMain() {
	utilLog(LogInfo, "Hello Rolling Modding World!");
	return 0;
}
