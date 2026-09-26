#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <ml/types.h>

// The max amount of files which can be open at once.
#define FILEMAN_MAX_FILES 8

// file.cpp
class File;

File* FileMan_openFile(const char* pszFileName);
void FileMan_closeFile(File* pFile);

#endif
