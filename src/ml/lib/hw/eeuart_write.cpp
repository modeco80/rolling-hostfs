#include "eeuart.h"
#include "eeuart_hw.h"

bool eeUartWritable() {
#ifdef PCSX2_HACKS
	// PCSX2 does not implement a proper TX FIFO; rather,
	// it immediately writes out the character, and does not signal writability or
	// if the FIFO is full. You can't see this, because it will only display TX when
	// a newline sequence (like '\r\n' or '\n') is written.
	return true;
#else
	return (*(volatile u32*)EEUART_REG_ISR) & EEUART_ISR_WRITABLE_MASK == EEUART_ISR_WRITABLE;
#endif
}

void eeUartNonblockingPutc(char c) {
	*((volatile u8*)EEUART_REG_TX_FIFO) = c;
}

void eeUartPutc(char c) {
#ifndef PCSX2_HACKS
	// Block waiting for the TX FIFO to be writable.
	while(!eeUartWritable());
#endif

	*((volatile u8*)EEUART_REG_TX_FIFO) = c;
}

void eeUartWrite(const char* pBuffer, usize bufferLength) {
	for(usize i = 0; i < bufferLength; ++i)
		eeUartPutc(pBuffer[i]);
}

void eeUartPuts(const char* pszIn) {
	// FIXME: Rewrite this to use eeUartWrite()
	while(*pszIn)
		eeUartPutc(*pszIn++);
	// act like real libc puts() and add a newline
	eeUartPutc('\n');
}
