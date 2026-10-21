#ifndef FILE_HPP
#define FILE_HPP

#include <ml/types.h>

/// Wrapper over EE FIO which is a bit easier to use and adds buffering.
/// This class is expected to be used with an previously opened file descriptor.
/// When in doubt, use the FileMan* APIs to get a File instance with a pre-opened file.
class File {
	i32 fd;
	u32 fileSize;

	// Read buffer state
	u32 readBufferPosition;
	u32 readBufferAvailable;
	u32 readBufferStart;
	u8* readBuffer;

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
