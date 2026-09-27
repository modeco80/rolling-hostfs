#include <sce/cdvd.h>

#include <ml/mem.h>
#include <ml/string.h>

#include "file.hpp"
#include "filemanager.hpp"
#include <utils/hook/fnhook.hpp>
#include <utils/log.hpp>

// These hooks define a "fake" version of the sceCdSt* API functions,
// which are fully compatible in place with the original APIs, but
// use our FileMan_*/File APIs, which are hostfs.

//#define CDST_DEBUG

// Fake version of the sceCdlFILE struct we return.
struct sceCdFile {
	const char* pszName;
	u32 size;

	char name[16];
	u8 date[8];
	u32 flag;
};

char gszCurrentCdStreamFileName[64];
File* gpCurrentCdStreamFile = nil(File*);

FUNC_REPLACE(sceCdSearchFile, i32, sceCdFile* pfile, const char* path) {
	File* pHostFile = FileMan_openFile(path);
	if(pHostFile == nil(File*))
		return -1;

	// Copy the name to our temporary file name buffer.
	memcpy(&gszCurrentCdStreamFileName[0], path, strlen(path)+1);

	// Set up the fake cdlfile struct.
	pfile->pszName = gszCurrentCdStreamFileName;
	pfile->size = pHostFile->getSize();

	FileMan_closeFile(pHostFile);
	return 1;
}

FUNC_REPLACE(sceCdStInit, i32, u32 maxBuffers, u32 maxBanks, u32 iopBuffer) {
#ifdef CDST_DEBUG
	utilLog(LogInfo, "sceCdStInit() called");
#endif
	return 1;
}

FUNC_REPLACE(sceCdStStart, i32, const char* pszFileName, void* rmode) {
#ifdef CDST_DEBUG
	utilLogf(LogInfo, "sceCdStStart() active, filename is %s", pszFileName);
#endif

	// Open the CD stream file.
	gpCurrentCdStreamFile = FileMan_openFile(pszFileName);
	// This shouldn't fail, but ToCToU safety is a good thing.
	if(gpCurrentCdStreamFile == nil(File*))
		return 0;

	return 1;
}

FUNC_REPLACE(sceCdStRead, i32, u32 sectorCount, u32* buf, u32 mode, u32* err) {
	if(gpCurrentCdStreamFile == nil(File*)) {
		// no stream active
#ifdef CDST_DEBUG
		utilLogf(LogInfo, "sceCdStRead(): No active stream");
#endif
		return -1;
	}

	if(gpCurrentCdStreamFile->eof()) {
#ifdef CDST_DEBUG
		utilLogf(LogInfo, "sceCdStRead(): Stream has ended");
#endif
		return -1;
	}

	i32 nRead = gpCurrentCdStreamFile->read(reinterpret_cast<u8*>(&buf[0]), sectorCount * 0x800);
	// Return the amount of sectors actually read
	return nRead / 0x800;
}

FUNC_REPLACE0(sceCdStStop, i32) {
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
