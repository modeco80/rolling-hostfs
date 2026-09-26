#ifndef ML_TYPES_H
#define ML_TYPES_H

// Shared types for all of the API
// This file is intended to be used in both C and C++ code.
// Please leave any C++ out or #if __cplusplus the C++ code.

#include <stddef.h>

// The C Standard <stdbool.h> header defines a boolean type which
// is fundamentally incompatible with the C++ bool type, since
// it is always sizeof(int), wherehas sizeof(bool) in C++ is always
// sizeof(u8) (or in other words, just 1 byte).
// Later C standards fix this, but C99 doesn't.
//
// Thanks, C. More like fuck you.
//
// Use this type across ABI boundaries of C++ code which passes booleans.
// You may still use stdbool true/false for them.
#ifndef __cplusplus
	#include <stdbool.h>
typedef unsigned char cxxbool;
#else
typedef bool cxxbool;
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
// TODO: u128 probably ????
typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;
// TODO: i128 probably ????

typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t isize;

// Primitive type X-macro list.
#define ML_PRIMITIVE_TYPE_LIST() \
	X(u8)                        \
	X(i8)                        \
	X(u16)                       \
	X(i16)                       \
	X(u32)                       \
	X(i32)                       \
	X(u64)                       \
	X(i64)

// A typed null pointer. I prefer this over the C NULL macro,
// since it is.. well, typed. It's possible to create a franken-nullptr,
// but I don't want to risk it being not optimized out.
#define nil(T) (T)0
#define vnil ((void*)0)

#define ml_weak __attribute__((weak))
#define ml_noreturn __attribute__((noreturn))
#define ml_align(n) __attribute__((aligned(n)))

// These defines do nothing, but provide
// keywords which are useful for grepping and such.
#define ml_override
#define ml_unsafe

/// Defines a variable which has the type of the expression.
#define ml_autovar(name, xpr) __typeof__((xpr)) name = xpr

#define ml_offsetof(T, Member) ((usize)&(((nil(T*))->Member)))

#define __ML_STATIC_ASSERT_CONCAT_HELPER(a, b) a##b
#define __ML_STATIC_ASSERT_CONCAT(a, b) __ML_STATIC_ASSERT_CONCAT_HELPER(a, b)
#define ML_STATIC_ASSERT(x) typedef u8 __ML_STATIC_ASSERT_CONCAT(__static_assert, __LINE__)[(x) ? 0 : -1]
#endif
