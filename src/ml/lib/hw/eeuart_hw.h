#ifndef EE_UART_HW_H
#define EE_UART_HW_H

//! Hardware specific EE UART defines.

/// PCSX2 does not emulate the EE UART very well.
///
/// This define adjusts the implementation
/// of code relying on these hardware-specific definitions
/// to work slightly better under it.
///
/// Hopefully, at some point, the EE UART will be emulated better
/// and this define will be able to be removed.
#define PCSX2_HACKS

/// Hardware address defines.
enum {
	EEUART_REG_ISR = 0x1000f130,
	EEUART_REG_TX_FIFO = 0x1000f180,
	EEUART_REG_RX_FIFO = 0x1000f1c0,

	// don't ask me dude
	EEUART_ISR_READABLE_MASK = 0xf00,
	EEUART_ISR_WRITABLE = 0x8000,
	EEUART_ISR_WRITABLE_MASK = 0xf000
};

#endif
