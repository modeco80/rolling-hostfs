#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/string.h>

char* mlStrDup(const char* pszString) {
	mlASSERT(pszString);
	if(!pszString)
		return nil(char*);

	u32 stringLength = strlen(pszString);
	char* pDuplicatedString = (char*)mlMalloc(stringLength + 1);

	mlASSERT(pDuplicatedString != nil(char*));
	if(!pDuplicatedString)
		return nil(char*);

	// Copy the string.
	memcpy(&pDuplicatedString[0], &pszString[0], stringLength);
	pDuplicatedString[stringLength] = '\0';
	return pDuplicatedString;
}
