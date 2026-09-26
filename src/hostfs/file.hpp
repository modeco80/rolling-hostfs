#ifndef FILE_HPP
#define FILE_HPP

#include <ml/types.h>

#define FIOMAN_DEBUG
#define FIOMAN_DEBUG_OPEN
//#define FIOMAN_DEBUG_READ // verbose as hell
//#define FIOMAN_DEBUG_SEEK


/// Wrapper over EE FIO which is a bit easier to use and adds buffering
class File {
	i32 fd;
	u32 fileSize;

	// Read buffer state
	u32 readBufferPosition;
	u32 readBufferAvailable;
	u32 readBufferStart;
	u8* readBuffer;

	void cacheSize();

public:

	explicit File(i32 fd);

	~File();
	i32 read(u8* pvBuf, i32 count);
	i32 tell();

	i32 seek(i32 offset, i32 whence);

	u32 getSize() const {
		return fileSize;
	}

	bool eof() {
		return tell() >= fileSize;
	}
};

#endif
