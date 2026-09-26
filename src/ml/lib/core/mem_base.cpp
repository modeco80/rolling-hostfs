#include <ml/mem.h>
#include <ml/string.h>

void* mlCalloc(u32 nmemb, u32 membsz) {
	void* p = mlMalloc(nmemb * membsz);
	if(!p)
		return vnil;
	memset(p, 0, nmemb * membsz);
	return p;
}
