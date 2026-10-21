#include <regdef.h>

#define at $1

/*
 * Minimal startup code for mod code.
 *
 * Where we are jumped into, we have a lot of stuff which has been pre-initialized for us
 * so we do not really need to do much of anything.
 */

.set noat
.set noreorder

.text

.global _start
_start:
	/* TODO: zerofill our .bss section. */

	/* Save registers which are clobbered on the stack. */
	addiu sp, sp, -0x8
	sw ra, (sp)
	sw gp, 4(sp)

	/* Call C++ global constructors. */
	jal __do_global_ctors
	nop

	/* Now that .bss should hopefully be filled, and
	 we've called global constructors, call the C++ entry point. */
	jal modMain
	nop

	/* Check the return value of modMain().
	  IF it is > 0, something went wrong and we execute global destructors. */
	slti at, v0, 0x0
	beq at, zero, .Lokay
	nop

	jal __do_global_dtors
	nop

.Lokay:
	/* Restore registers which were clobbered from the stack
	 and clean up the stack, preparing to return back to the game */
	lw gp, 4(sp)
	lw ra, (sp)
	addiu sp, sp, 0x8

	/* Execute the first two overwritten instructions of main() */
	addiu sp, sp, -0xa0

	/* Return back to the game code. */
	j mainTrampoline
	sq s0, (sp)

