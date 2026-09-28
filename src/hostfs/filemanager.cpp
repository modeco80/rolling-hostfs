#include "filemanager.hpp"

#include "filemanager_config.hpp"

#include <ml/abort.h>
#include <ml/mem.h>
#include <sce/fio.h>

#include <ml/cxx/freelist.hpp>
#include <new>

#include "file.hpp"
#include "utils/log.hpp"

// The max amount of files which can be open at once.
#define FILEMAN_MAX_FILES 8

namespace {
	ml::FreeList<File, FILEMAN_MAX_FILES> openFileList;

	void translateFileName(char* pszOut, const char* pszInPath) {
		mlStaticStrCpy(pszOut, "host0:");
		strcat(pszOut, pszInPath);

		const u32 len = strlen(pszOut);
		for(u32 i = sizeof("host0:") - 1; i < len; ++i) {
			// Clean up the case of file paths so it is entirely lowercase.
			if(pszOut[i] >= 'A' && pszOut[i] <= 'Z')
				pszOut[i] |= 0x20;

			// Fix up the objectively incorrect directory seperator to the correct one.
			if(pszOut[i] == '\\')
				pszOut[i] = '/';
		}
	}
} // namespace

File* FileMan_openFile(const char* path) {
	File* pFile = openFileList.allocate();
	if(pFile == nil(File*)) {
		utilLog(LogErr, "FileMan: We somehow exhausted all file handles???");
		return nil(File*);
	}

	char translatedPath[512];
	translateFileName(&translatedPath[0], path);

#if defined(FILEMAN_DEBUG) && defined(FILEMAN_DEBUG_OPEN)
	utilLogf(LogInfo, "HostFS Open %s", translatedPath);
#endif

	i32 fd = sceOpen(translatedPath, SCE_RDONLY);
	if(fd < 0) {
#if defined(FILEMAN_DEBUG) && defined(FILEMAN_DEBUG_OPEN)
		utilLogf(LogErr, "HostFS Open FAIL %s", translatedPath);
#endif
		openFileList.free(pFile);
		return nil(File*);
	}

	// ml freelists don't new objects, so we have to do it ourselves.
	return new(pFile) File(fd);
}

void FileMan_closeFile(File* pFile) {
	pFile->~File();
	openFileList.free(pFile);
}
