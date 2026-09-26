#ifndef UTILS_HOOK_TRAMPOLINE_HPP
#define UTILS_HOOK_TRAMPOLINE_HPP

#include <ml/types.h>

typedef void* HookHandle;

/// Does a trampoline hook at the target function pointed to by [pTarget].
/// [pHook] is a pointer to the hook function.
///
/// If [ppTrampoline] is not nil, the pointer to a trampoline thunk which allows
/// calling the original code again will be provided.
///
/// Returns a hook handle, or nil(HookHandle) on error.
///
/// # Notes
/// There is no "thiscall" variant, because thiscall on MIPS does not meaningfully alter calling convention.
/// Therefore, this function can be used to hook class member functions AND regular functions without any issues.
HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline);

/// Unhooks a function previously hooked with [trampolineHook()].
void trampolineUnhook(HookHandle hook);

#endif
