#ifndef ML_API_STRING_H
#define ML_API_STRING_H

#include <ml/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 strlen(const char* pszString);

void* memset(void* dst, i32 c, usize len);

void* memcpy(void* dst, const void* src, u32 len);

// TODO:
// - strcpy
// - strcat
// - sprintf

// bit of a hack, but saves needing strcpy directly in cases where
// you only need the literal :)
#define mlStaticStrCpy(dst, srcStrLiteral) memcpy(dst, &(srcStrLiteral)[0], sizeof(srcStrLiteral))

void strcat(char* dst, const char* src);

int vsprintf(char* pszOut, const char* pszFormat, va_list val);
int sprintf(char* out, const char* format, ...);

/// Duplicates a string on the heap. The memory must be freed with
/// the [mlFree()] function once the duplicated string is no longer needed.
char* mlStrDup(const char* pszString);

#ifdef __cplusplus
}
#endif

#endif
