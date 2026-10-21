#ifndef SCE_CDVD_H
#define SCE_CDVD_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sceCdFile;

i32 sceCdSearchFile(sceCdFile* fp, const char* name);

// Streaming
int sceCdStInit(u32 bufmax, u32 bankmax, u32 iop_bufaddr);
int sceCdStStart(u32 lbn, void* mode);
int sceCdStStop(void);
int sceCdStRead(u32 size, u32* buf, u32 mode, u32* err);

#ifdef __cplusplus
}
#endif

#endif
