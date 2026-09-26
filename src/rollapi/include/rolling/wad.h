#ifndef ROLLAPI_WAD_H
#define ROLLAPI_WAD_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// WAD Manager

extern i32 Wad_ReadCount;
extern i32 Wad_ReadAllFileSize;

void Wad_InstallFileSystem();
void Wad_Mount(const char* pszPath);
void Wad_Unmount();

// Stdio API projection for wad files.
void* Wad_fopen(const char* path, const char* mode);
void Wad_fclose(void* wadfile);

int Wad_fexist(const char* path);
int Wad_feof(void* wadfile);
int Wad_fflush(void* wadfile);

int Wad_fseek(void* wadfile, i32 offset, i32 whence);

int Wad_fgets(char* pszIn, i32 count, void* wadfile);
int Wad_fputs(const char* pszIn, void* wadfile);

int Wad_fread(void* pBuffer, i32 size, i32 nitems, void* wadfile);
int Wad_fwrite(const void* pBuffer, i32 size, i32 nitems, void* wadfile);

// Reads all of a file
void* Wad_ReadAll(const char* pszFileName);
int Wad_ReadAllInto(const char* pszFileName, void* pBuffer, i32 count);

#ifdef __cplusplus
}
#endif

#endif
