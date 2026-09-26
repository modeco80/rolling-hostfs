#ifndef UTILS_HOOK_FNHOOK_HPP
#define UTILS_HOOK_FNHOOK_HPP

#include "trampoline.hpp"

#define FUNC_HOOK0(funcName, ret)                                                             \
	static class Hook_##funcName {                                                            \
		HookHandle handle;                                                                    \
		ret (*original)();                                                                    \
		static ret hookImpl();                                                                \
                                                                                              \
	   public:                                                                                \
		bool hook() {                                                                         \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                                 \
		}                                                                                     \
	} hook_##funcName;                                                                        \
	ret Hook_##funcName::hookImpl()

#define FUNC_HOOK(funcName, ret, ...)                                                         \
	static class Hook_##className##_##funcName {                                              \
		HookHandle handle;                                                                    \
		ret (*original)(__VA_ARGS__);                                                         \
		static ret hookImpl(__VA_ARGS__);                                                     \
                                                                                              \
	   public:                                                                                \
		bool hook() {                                                                         \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                                 \
		}                                                                                     \
	} hook_##funcName;                                                                        \
	ret Hook_##funcName::hookImpl(__VA_ARGS__)

#endif
