#ifndef ML_API_IO_H
#define ML_API_IO_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	// seek mode
	ML_SEEK_BEG = 0,
	ML_SEEK_CUR,
	ML_SEEK_END,

	// open mode
	ML_OPEN_READ = (1 << 0),
	ML_OPEN_WRITE = (1 << 1),
	ML_OPEN_RDWR = ML_OPEN_READ | ML_OPEN_WRITE
};

/// A directory entry's type.
typedef enum {
	ML_DIRENT_FILE = 0,
	ML_DIRENT_DIRECTORY = 1
} mlIoDirEntryType;

typedef struct mlIoDirEnt {
	mlIoDirEntryType dirEntityType;
	char dirEntityName[64];
	// TODO: more data? For now, this is good enough
} mlIoDirEnt;

/// Opens a file.
i32 mlIoOpen(const char* pszPath, u32 openMode);

/// Read some data from the file opened as [fd].
i32 mlIoRead(i32 fd, void* buffer, u32 count);

/// Write some data to the file opened as [fd].
i32 mlIoWrite(i32 fd, const void* buffer, u32 count);

// Seeking APIs.
i32 mlIoTell(i32 fd);
i32 mlIoLseek(i32 fd, i32 offset, i32 whence);

void mlIoClose(i32 fd);

// Directory APIs.

/// Opens a directory.
i32 mlIoDirOpen(const char* pszPath);

/// Reads a directory. Returns the amount of directory entries written to [dirBuf],
/// 0 on end-of-directory, or -1 on error.
i32 mlIoDirRead(i32 dirfd, mlIoDirEnt* pEntries, u32 countEntries);

// Rewinds the directory. mlDirRead() will re-read existing directory entries.
void mlIoDirRewind(i32 dirfd);

/// Closes a directory.
void mlIoDirClose(i32 dirfd);

// TODO:
// - mlPrintf(const char*, ...);

#ifdef __cplusplus
namespace ml {
	/// Inline wrapper of the mlIo file APIs to make using them a bit cleaner,
	/// and less error-prone.
	///
	/// Example usage:
	/// ```cpp
	/// ml::IoFile file("host:my_file.bin", ml::IoFile::Read);
	/// if(file.ok()) {
	///     char buf[8];
	///     i32 count = file.read(&buf[0], sizeof(buf));
	///     if(count == sizeof(buf)) {
	///         // It worked!
	///     }
	/// }
	/// ```
	class IoFile {
		i32 fd;

	   public:
		enum OpenMode {
			Read = ML_OPEN_READ,
			Write = ML_OPEN_WRITE,
			ReadWrite = ML_OPEN_RDWR
		};

		IoFile(const char* pszPath, u32 openMode) {
			fd = mlIoOpen(pszPath, openMode);
		}

		~IoFile() {
			close();
		}

		bool ok() const {
			return fd != -1;
		}

		i32 read(void* pvBuffer, u32 count) {
			mlASSERT(ok() && "FILE IS NOT GOOD!");
			return mlIoRead(fd, pvBuffer, count);
		}

		i32 write(const void* pvBuffer, u32 count) {
			return mlIoWrite(fd, pvBuffer, count);
		}

		i32 tell() {
			return mlIoTell(fd);
		}

		i32 seek(i32 offset, i32 whence) {
			return mlIoLseek(fd, offset, whence);
		}

		void close() {
			if(fd != -1)
				mlIoClose(fd);
		}
	};

} // namespace ml
#endif

#ifdef __cplusplus
}
#endif

#endif
