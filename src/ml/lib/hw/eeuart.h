#ifndef EE_UART_H
#define EE_UART_H

#include <ml/string.h>
#include <ml/types.h>

//! This header declares helper utilities for using the EE UART periphial.

/// Return values of eeUartPollGetLine().
enum {
	/// The line was read.
	EEUART_GETLINE_GOT_LINE = 0,

	EEUART_GETLINE_INVALID_BUFFER,

	/// Indicates that eeUartPollGetLine() emptied the FIFO.
	EEUART_GETLINE_EMPTIED_FIFO,

	/// Indicates that the read FIFO was empty when eeUartPollGetLine() was called.
	EEUART_GETLINE_FIFO_EMPTY,

	/// Indicates that the buffer is too small to hold the line.
	/// It will need to be reallocated.
	EEUART_GETLINE_BUFFER_TOO_SMALL
};

/// Line buffer structure.
typedef struct eeUartLineBuffer {
	char* pBuffer;
	u32 bufferLength;
	u32 bufferWriteIndex;
#ifdef __cplusplus
	eeUartLineBuffer() {
		memset(this, 0, sizeof(*this));
	}

	// helper constructor to make initalizing this structure less error prone
	eeUartLineBuffer(char* pBuffer, u32 bufferLength)
		: pBuffer(pBuffer), bufferLength(bufferLength) {
		rewind();
	}

	void rewind() {
		bufferWriteIndex = 0;
	}
#endif
} eeUartLineBuffer;

/// Returns true if the RX FIFO has characters in it
/// (or in other words, is readable.)
cxxbool eeUartReadable();

/// Returns true if the TX FIFO is writable.
cxxbool eeUartWritable();

/// Gets a character without blocking.
/// IMPORTANT: You must verify before calling this function
/// that eeUartReadable() returns true.
char eeUartNonblockingGetc();

/// Gets a character, blocking waiting for the RX FIFO.
char eeUartGetc();

void eeUartEmptyRxFifo();

/// A non-blocking getline(). For example usage, see the implementation
/// of eeUartGetLine().
u32 eeUartPollGetLine(eeUartLineBuffer* pBuffer);

/// Blocking wrapper of eeUartPollGetLine().
u32 eeUartGetLine(char* pBuffer, usize bufferLength);

void eeUartNonblockingPutc(char c);

void eeUartPutc(char c);

void eeUartWrite(const char* pBuffer, usize bufferLength);

void eeUartPuts(const char* pszIn);

#endif
