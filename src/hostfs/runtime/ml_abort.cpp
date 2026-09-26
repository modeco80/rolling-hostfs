#include <ml/types.h>

#include "../utils/log.hpp"

extern "C" ml_noreturn void mlAbort(const char* msg) {
	utilLogf(LogErr, "ML abort: %s", msg);
	// maybe dump stack?
	// Spin forever to uphold never returning
	while(1) {
	}
}

#ifndef NDEBUG
extern "C" ml_noreturn void mlAbortAssertionFailure(const char* pszExpression, const char* file, i32 line) {
	utilLogf(LogErr, "Assertion \"%s\" failed in %s:%d", pszExpression, file, line);
	// maybe dump stack?
	// Spin forever to uphold never returning
	while(1) {
	}
}
#endif
