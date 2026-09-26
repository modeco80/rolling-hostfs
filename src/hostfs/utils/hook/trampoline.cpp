#include "trampoline.hpp"

#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/types.h>

#include <ml/cxx/freelist.hpp>

#include "mips.hpp"

/// Per-hook saved data.
struct ml_align(4) HookData {
	/// The original target, which we use to later restore
	/// the original instructions when unhooking.
	void* pTarget;

	/// A 4-instruction long trampoline which can call the original function.
	u32 trampoline[4];
};

namespace {
	// Only bump this up if we reallllly have to :(
	ml::FreeList<HookData, 32> hookList;

	inline HookData* allocHook(void* pTarget) {
		HookData* hook = hookList.allocate();
		if(hook == nil(HookData*)) {
			mlASSERT(false && "Out of free hooks, increase the freelist size!");
			return nil(HookData*);
		}

		// Initialize the hook structure that we got with the target
		hook->pTarget = pTarget;
		return hook;
	}

	inline void freeHook(HookData* hook) {
		hookList.free(hook);
	}

} // namespace

HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline) {
	// Idiot checks which don't need to be done in release.
	mlASSERT(pTarget != nil(void*));
	mlASSERT(pHook != nil(void*));

	// Try to allocate a hook structure first. If this fails, then
	// return a nil handle to indicate that we couldn't hook.
	HookData* hook = allocHook(pTarget);
	if(hook == nil(HookData*))
		return nil(HookHandle);

	volatile u32* targetInstructions = reinterpret_cast<u32*>(pTarget);

	// TRAMPOLINE SETUP
	volatile u32* pTrampolineBuf = &hook->trampoline[0];

	// Copy the original instructions to the start of the trampoline buffer
	*pTrampolineBuf++ = targetInstructions[0];
	*pTrampolineBuf++ = targetInstructions[1];

	// Encode the jump back to the original function body
	*pTrampolineBuf++ = mipse::j(reinterpret_cast<u32>(reinterpret_cast<u32*>(pTarget) + 2)); // j [pTarget+8] (to skip the hook)
	*pTrampolineBuf++ = 0x0;																  // nop

	// In the target, encode the hook to our code
	*targetInstructions++ = mipse::j(reinterpret_cast<u32>(pHook)); // j [pHook]
	*targetInstructions++ = 0x0;									// nop

	// If the user passes a valid pointer, give them a pointer to the trampoline buffer
	// that they can call if they want to call the original function
	if(ppTrampoline) {
		*ppTrampoline = reinterpret_cast<void*>(&hook->trampoline[0]);
	}

	// Flush caches.
	// FIXME: Having a syscall wrapper (or linking the game's) for this may be a hair nicer
	__asm__ volatile(
	"li $3, 0x64\n"
	"ori $4, $0, 0x0\n"
	"syscall\n" // FlushCache(0)
	"li $3, 0x64\n"
	"ori $4, $0, 0x2\n"
	"syscall\n" // FlushCache(2)
	);

	return reinterpret_cast<HookHandle>(hook);
}

void trampolineUnhook(HookHandle hook) {
	mlASSERT(hook != nil(HookHandle));
	if(hook != nil(HookHandle)) {
		HookData* pHook = reinterpret_cast<HookData*>(hook);

		// Copy the original instructions back to the target.
		ml_autovar(pTarget, reinterpret_cast<u32*>(pHook->pTarget));
		pTarget[0] = pHook->trampoline[0];
		pTarget[1] = pHook->trampoline[1];

		// Flush caches.
		__asm__ volatile(
		"li $3, 0x64\n"
		"ori $4, $0, 0x0\n"
		"syscall\n" // FlushCache(0)
		"li $3, 0x64\n"
		"ori $4, $0, 0x2\n"
		"syscall\n" // FlushCache(2)
		);

		// Free the hook.
		freeHook(pHook);
	}
}
