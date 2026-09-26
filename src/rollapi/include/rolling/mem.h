#ifndef ROLLAPI_MEM_H
#define ROLLAPI_MEM_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

	void* memAllocAligned(u32 count, i32 align);

#ifdef __cplusplus
}
#endif

#endif
