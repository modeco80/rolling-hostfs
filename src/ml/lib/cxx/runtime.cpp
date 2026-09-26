#include <ml/abort.h>
#include <ml/types.h>

// This provides implementation of some GCC C/C++
// runtime stuff that's needed, *without* dragging in
// libgcc, since that requires a nonzero part of libc to also be dragged in.
//
// We make the choice not to link to libc, but instead
// we reimplement a slightly cleaner surface in the core,
// which can take from libc symbols found in the game, or
// the game's custom implementations (for e.g: memory allocation)

extern "C" {
ml_noreturn void __pure_virtual() {
	mlAbort("pure virtual function called");
}

typedef void (*vfunc)();

// Global constructors
extern vfunc __CTOR_LIST__[];
extern vfunc __DTOR_LIST__[];

void __do_global_ctors() {
	u32 nrPtrs = (u32)__CTOR_LIST__[0];

	// A -1 value means that the list size has to be calculated
	// by this function.
	if(nrPtrs == (u32)0xffffffff) {
		for(nrPtrs = 0; __CTOR_LIST__[nrPtrs + 1] != 0; nrPtrs++);
	}

	// Call constructors in reverse order.
	for(u32 i = nrPtrs; i >= 1; i--) {
		__CTOR_LIST__[i]();
	}
}

void __do_global_dtors() {
	static vfunc* ppfnDtor = &__DTOR_LIST__[1];
	while(*ppfnDtor) {
		ppfnDtor++;
		(*(ppfnDtor - 1))();
	}
}

#if 0
    // This function call is emitted by GCC in C++ for targets
    // which do not support .init/.fini sections. The PS2's relatively
    // basic ELF loader (EELOAD) doesn't support these, so it uses this.
    //
    // This actually makes our lives easier for making this a standalone blob,
    // so... yay?
    void __main() {
        __do_global_ctors();
    }
#endif
}
