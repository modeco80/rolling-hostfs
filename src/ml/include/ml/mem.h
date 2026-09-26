#ifndef ML_API_MEMORY_H
#define ML_API_MEMORY_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Allocates some memory.
void* mlMalloc(u32 size);

/// Frees said memory.
void mlFree(void* pMemory);

/// Allocates cleared (zero-filed) memory.
void* mlCalloc(u32 nmemb, u32 membsz);

#ifdef __cplusplus
}
#endif

#endif
