#include "eeuart.h"

u32 eeUartPollGetLine(eeUartLineBuffer* pLineBuffer) {
	if(pLineBuffer->pBuffer)
		return EEUART_GETLINE_BUFFER_TOO_SMALL;

	if(eeUartReadable()) {
		// Empty the RX FIFO of characters.
		while(eeUartReadable()) {
			char fifoCharacter = eeUartNonblockingGetc();
			bool gotLine = false;

			// We got a newline.
			if(fifoCharacter == '\n') {
				fifoCharacter = '\0';
				gotLine = true;
			}

			// Check to make sure we won't overflow the buffer. If we do try to write too far,
			// don't, and return an error to the user.
			if(pLineBuffer->bufferWriteIndex + 1 > pLineBuffer->bufferLength) {
				pLineBuffer->rewind();
				return EEUART_GETLINE_BUFFER_TOO_SMALL;
			}

			// Write the character from the RX FIFO to the buffer.
			pLineBuffer->pBuffer[pLineBuffer->bufferWriteIndex++] = fifoCharacter;

			// If we got the line, break out and tell the user so they can process it.
			if(gotLine) {
				pLineBuffer->rewind();
				return EEUART_GETLINE_GOT_LINE;
			}
		}

		// We emptied the FIFO ourselves, but didn't capture a line yet.
		return EEUART_GETLINE_EMPTIED_FIFO;
	} else {
		// Sad case: the RX FIFO is currently empty. Immediately bail out.
		return EEUART_GETLINE_FIFO_EMPTY;
	}
}

u32 eeUartGetLine(char* pBuffer, usize bufferLength) {
	eeUartLineBuffer buffer(pBuffer, bufferLength);

	while(true) {
		u32 result = eeUartPollGetLine(&buffer);
		switch(result) {
			case EEUART_GETLINE_GOT_LINE:
				return 0;
				break;

			// RX FIFO empty, we just try again
			case EEUART_GETLINE_EMPTIED_FIFO:
			case EEUART_GETLINE_FIFO_EMPTY:
				break;

			case EEUART_GETLINE_BUFFER_TOO_SMALL:
				return EEUART_GETLINE_BUFFER_TOO_SMALL;
				break;
		}
		// TODO: probably some slight delay so we dont just hammer the cpu?
	}
}
