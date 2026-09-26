#ifndef CHAOS_HOOK_CLASSHOOK_HPP
#define CHAOS_HOOK_CLASSHOOK_HPP

// Class hooking helpers.

#include "trampoline.hpp"

#define CLASS_HOOK0(ret, className, funcName)                                          \
	static class Hook_##className##_##funcName {                                       \
		HookHandle handle;                                                             \
		ret (*original)(className * klass);                                            \
		static ret hookImpl(className* klass);                                         \
		typedef ret (*RawPfn)(className*);                                             \
                                                                                       \
	   public:                                                                         \
		bool hook() {                                                                  \
			void* barePointer = (void*)((RawPfn) & className::funcName);               \
			handle = trampolineHook(barePointer, (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                          \
		}                                                                              \
	} hook_##className##_##funcName;                                                   \
	ret Hook_##className##_##funcName::hookImpl(className* klass)

#define CLASS_HOOK(ret, className, funcName, ...)                                      \
	static class Hook_##className##_##funcName {                                       \
		HookHandle handle;                                                             \
		ret (*original)(className * klass, __VA_ARGS__);                               \
		static ret hookImpl(className* klass, __VA_ARGS__);                            \
		typedef ret (*RawPfn)(className*, __VA_ARGS__);                                \
                                                                                       \
	   public:                                                                         \
		bool hook() {                                                                  \
			void* barePointer = (void*)((RawPfn) & className::funcName);               \
			handle = trampolineHook(barePointer, (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                          \
		}                                                                              \
	} hook_##className##_##funcName;                                                   \
	ret Hook_##className##_##funcName::hookImpl(className* klass, __VA_ARGS__)

#endif
