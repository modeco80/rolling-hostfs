#ifndef FILEMANAGER_CONFIG_HPP
#define FILEMANAGER_CONFIG_HPP

#define FILEMAN_DEBUG
#define FILEMAN_DEBUG_OPEN
// #define FILEMAN_DEBUG_READ // verbose as hell
// #define FILEMAN_DEBUG_SEEK

/// the size of the read buffer inside each File class instance
/// Try to keep this sensible. 16 KB is a decent default size.
#define FILEMAN_READ_BUFFER_SIZE 0x1000

#endif
