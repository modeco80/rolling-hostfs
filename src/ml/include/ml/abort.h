#ifndef ML_API_ABORT_H
#define ML_API_ABORT_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Aborts.
ml_noreturn void mlAbort(const char* pszMsg);
#ifndef NDEBUG
ml_noreturn void mlAbortAssertionFailure(const char* pszExpression, const char* file, i32 line);
#endif

#define ___assert_stringify(xx) #xx
#define __assert_stringify(x) ___assert_stringify(x)

#ifdef NDEBUG
	#define mlASSERT(xpr) (void)0
#else
	#define mlASSERT(xpr)                                                         \
		if(!(xpr)) {                                                              \
			mlAbortAssertionFailure(__assert_stringify(xpr), __FILE__, __LINE__); \
		}
#endif

#ifdef __cplusplus
}
#endif

#endif
