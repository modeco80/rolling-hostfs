#include "file.hpp"

#include <ml/mem.h>
#include <ml/string.h>
#include <sce/fio.h>

/// the size of the read buffer inside each FioFile instance
/// Try to keep this sensible
#define FIOMAN_READ_BUFFER_SIZE 0x800

void File::cacheSize() {
	sceLSeek(fd, 0, SCE_SEEK_END);
	fileSize = sceLSeek(fd, 0, SCE_SEEK_CUR);
	sceLSeek(fd, 0, SCE_SEEK_SET);
}

File::File(i32 fd)
	: fd(fd) {
	cacheSize();

	// Reset buffer state.
	readBufferAvailable = 0;
	readBufferPosition = 0;
	readBufferStart = 0;
	readBuffer = reinterpret_cast<u8*>(mlMalloc(FIOMAN_READ_BUFFER_SIZE));
}

File::~File() {
	mlFree(readBuffer);
	sceClose(fd);
}

i32 File::read(u8* pvBuf, i32 count) {
	i32 total = 0;
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
	utilLogf(LogInfo, "File::read(count: %d)", count);
#endif

	while(count > 0) {
		// Check if the buffer has been used up. If so, then we need to read again.
		if(readBufferPosition == readBufferAvailable) {
			u32 offset = readBufferStart + readBufferPosition;
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
			utilLogf(LogInfo, "File::read() Need to seek to %d to service read buffer", offset);
#endif
			sceLSeek(fd, offset, SCE_SEEK_SET);

			readBufferAvailable = sceRead(fd, &readBuffer[0], FIOMAN_READ_BUFFER_SIZE);

			readBufferStart = offset;
			readBufferPosition = 0;

			if(readBufferAvailable == 0)
				break;
		}

		u32 avail = readBufferAvailable - readBufferPosition;
		u32 n = mlMin(count, avail);

		memcpy(pvBuf, &readBuffer[readBufferPosition], n);

		pvBuf += n;
		readBufferPosition += n;
		count -= n;
		total += n;
	}

#if 0 // defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_READ)
	if(count == 0)
		utilLogf(LogInfo, "File::read() read all %d bytes", total);
	else
		utilLogf(LogInfo, "File::read() read %d bytes, %d were not read", total, count);
#endif

	return total;
}

i32 File::tell() {
	return (readBufferStart + readBufferPosition);
}

i32 File::seek(i32 offset, i32 whence) {
	i32 current = readBufferStart + readBufferPosition;
	i32 target;

#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
	utilLogf(LogInfo, "File::seek(offset: %d, whence: %d)", offset, whence);
#endif

	switch(whence) {
		case 0: // relative to begin of file
			target = offset;
			break;
		case 1: // relative to current position
			target = current + offset;
			break;
		case 2: // relative to end of file.
			// If the offset is not negative or 0 then give up
			if(offset != 0 && offset > 0)
				return -1;
			target = fileSize + offset;
			break;
	}

	if(target > fileSize) {
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
		utilLogf(LogErr, "File::seek() Invalid target %d", target);
#endif
		return -1;
	}

	if(target >= readBufferStart && target <= readBufferStart + readBufferAvailable) {
		readBufferPosition = static_cast<i32>(target - readBufferStart);
	} else {
#if defined(FIOMAN_DEBUG) && defined(FIOMAN_DEBUG_SEEK)
		utilLogf(LogInfo, "File::seek() Abandoning buffer");
#endif
		// Discard buffer.
		readBufferStart = target;
		readBufferAvailable = 0;
		readBufferPosition = 0;
		sceLSeek(fd, target, whence);
	}

	return 0;
}
