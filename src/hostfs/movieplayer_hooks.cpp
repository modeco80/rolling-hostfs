#include <sce/cdvd.h>

#include "file.hpp"
#include "filemanager.hpp"
#include "utils/hook/fnhook.hpp"
#include "utils/log.hpp"

// These hooks define a "fake" version of the sceCdSt* API functions,
// which are fully compatible in place with the original APIs, but
// use our FileMan_*/File APIs, which are hostfs.

// #define CDST_DEBUG

// Fake version of the sceCdlFILE struct we return.
struct sceCdFile {
	File* pFile;
	u32 size;

	char name[16];
	u8 date[8];
	u32 flag;
};

File* gpCurrentCdStreamFile = nil(File*);

FUNC_HOOK(sceCdSearchFile, i32, sceCdFile* pfile, const char* path) {
	File* pHostFile = FileMan_openFile(path);
	if(pHostFile == nil(File*))
		return -1;

	// Set up the fake cdlfile struct. We get the pfile in sceCdStStart.
	pfile->pFile = pHostFile;
	pfile->size = pHostFile->getSize();
#ifdef CDST_DEBUG
	utilLogf(LogInfo, "sceCdSearchFile(): Host file is 0x%08x", pHostFile);
#endif
	return 1;
}

extern "C" void sceSifFreeIopHeap(u32);

FUNC_HOOK(sceCdStInit, i32, u32 maxBuffers, u32 maxBanks, u32 iopBuffer) {
#ifdef CDST_DEBUG
	utilLog(LogInfo, "sceCdStInit() called");
#endif
	return 1;
}

FUNC_HOOK(sceCdStStart, i32, File* pfile, void* rmode) {
#ifdef CDST_DEBUG
	utilLogf(LogInfo, "sceCdStStart() active, file is 0x%08x", pfile);
#endif
	gpCurrentCdStreamFile = pfile;
	return 1;
}

FUNC_HOOK(sceCdStRead, i32, u32 size, u32* buf, u32 mode, u32* err) {
	if(gpCurrentCdStreamFile == nil(File*)) {
		// no stream active
		return 0;
	}

	if(gpCurrentCdStreamFile->eof())
		return 0;

	gpCurrentCdStreamFile->read(reinterpret_cast<u8*>(&buf[0]), size * 0x800);
	return size;
}

FUNC_HOOK0(sceCdStStop, i32) {
#ifdef CDST_DEBUG
	utilLogf(LogInfo, "sceCdStStop()");
#endif
	if(gpCurrentCdStreamFile) {
		FileMan_closeFile(gpCurrentCdStreamFile);
		gpCurrentCdStreamFile = nil(File*);
	}
	return 1;
}

bool movieInitHooks() {
#define DO_HOOK_FUNC(fnName)                             \
	if(!hook_##fnName.hook()) {                          \
		utilLogf(LogErr, "Failed to hook %s.", #fnName); \
		return false;                                    \
	}

	DO_HOOK_FUNC(sceCdSearchFile);

	DO_HOOK_FUNC(sceCdStInit);
	DO_HOOK_FUNC(sceCdStStart);
	DO_HOOK_FUNC(sceCdStRead);
	DO_HOOK_FUNC(sceCdStStop);
	return true;
}
