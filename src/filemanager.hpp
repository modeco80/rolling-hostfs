#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <ml/types.h>
#include "file.hpp"

/// Open a file for reading from HostFS.
File* FileMan_openFile(const char* pszFileName);

/// Closes a file previously opened by FileMan_openFile().
void FileMan_closeFile(File* pFile);

#endif
