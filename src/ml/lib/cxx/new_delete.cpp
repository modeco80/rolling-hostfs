#include <ml/mem.h>
#include <ml/types.h>

#include <new>

void* operator new(usize len) {
	return mlMalloc(len);
}

void operator delete(void* ptr) {
	return mlFree(ptr);
}

void* operator new[](usize len) {
	return mlMalloc(len);
}

void operator delete[](void* ptr) {
	return mlFree(ptr);
}
