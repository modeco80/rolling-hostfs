#include "log.hpp"
#ifdef LOG_ENABLED

#include <ml/abort.h>
#include <ml/string.h>

#include "../../ml/lib/hw/eeuart.h"

inline const char* utilGetLogStamp(LogLevel level) {
	switch(level) {
		case LogInfo:
			return "[Info]";
		case LogWarn:
			return "[Warn]";
		case LogErr:
			return "[ERROR]";
		default:
			mlASSERT(false);
			break;
	}
}

void utilLog(LogLevel level, const char* pszMessage) {
	char buf[512];
	sprintf(&buf[0], "%s %s", utilGetLogStamp(level), pszMessage);
	eeUartPuts(&buf[0]);
}

void utilLogv(LogLevel level, const char* pszMessage, va_list val) {
	char formatbuf[256];
	vsprintf(&formatbuf[0], pszMessage, val);
	utilLog(level, &formatbuf[0]);
}

void utilLogf(LogLevel level, const char* pszMessage, ...) {
	va_list val;
	va_start(val, pszMessage);
	utilLogv(level, pszMessage, val);
	va_end(val);
}

#endif
