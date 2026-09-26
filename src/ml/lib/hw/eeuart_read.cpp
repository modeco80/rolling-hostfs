#include "eeuart.h"
#include "eeuart_hw.h"

bool eeUartReadable() {
	return ((*(volatile u32*)EEUART_REG_ISR) & EEUART_ISR_READABLE_MASK) != 0;
}

char eeUartNonblockingGetc() {
	return *((volatile i8*)EEUART_REG_RX_FIFO);
}

char eeUartGetc() {
	// Block waiting for the FIFO to have a character in it.
	while(!eeUartReadable());
	return *((volatile i8*)EEUART_REG_RX_FIFO);
}

void eeUartEmptyRxFifo() {
	while(eeUartReadable())
		static_cast<void>(eeUartNonblockingGetc());
}
