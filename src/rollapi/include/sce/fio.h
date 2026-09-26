#ifndef SCE_FIO_H
#define SCE_FIO_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_RDONLY      0x0001
#define SCE_WRONLY      0x0002
#define SCE_RDWR        0x0003
#define SCE_NBLOCK      0x0010
#define SCE_APPEND      0x0100
#define SCE_CREAT       0x0200
#define SCE_TRUNC       0x0400
#define SCE_EXCL        0x0800

#define SCE_SEEK_SET        (0)
#define SCE_SEEK_CUR        (1)
#define SCE_SEEK_END        (2)

i32 sceOpen(const char* filename, i32 mode, ...);
i32 sceLSeek(i32 fd, i32 where, i32 whence);
i32 sceRead(i32 fd, void* pvBuffer, i32 count);
void sceClose(i32 fd);

#ifdef __cplusplus
}
#endif

#endif
