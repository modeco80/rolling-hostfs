#include <ml/mem.h>
#include <ml/string.h>

#include <ml/cxx/string.hpp>

namespace ml {

	String::String() {
		mem = nil(char*);
		length = 0;
	}

	String::String(const char* str) {
		assign(str);
	}

	String::String(const String& str) {
		assign(str);
	}

	String::~String() {
		clear();
	}

	String& String::operator=(const String& other) {
		if(&other == this)
			return *this;
		assign(other);
		return *this;
	}

	String& String::operator=(const char* str) {
		assign(str);
		return *this;
	}

	void String::assign(const String& str) {
		resize(str.getLength());
		memcpy(&mem[0], &str.mem[0], str.getLength());
	}

	void String::assign(const char* str) {
		if(!str)
			return;
		u32 len = strlen(str);
		resize(len);
		memcpy(&mem[0], str, len);
	}

	void String::resize(u32 len) {
		if(len >= capacity) {
			reserve(len);
		} else {
			mem[len] = 0;
		}

		length = len;
	}

	void String::reserve(u32 newCapacity) {
		if(capacity == 0) {
			mem = (char*)mlMalloc(newCapacity + 1);
			mem[newCapacity] = 0;
			capacity = newCapacity;
		}

		if(newCapacity >= capacity) {
			char* memOld = mem;
			mem = (char*)mlMalloc(newCapacity);

			if(memOld) {
				memcpy(&mem[0], &memOld[0], length);
				mlFree(memOld);
			}

			capacity = newCapacity;
		}
	}

	void String::clear() {
		if(mem) {
			mlFree(mem);
			length = 0;
			capacity = 0;
			mem = nil(char*);
		}
	}

	bool String::empty() const {
		if(capacity != 0) {
			return length == 0;
		}

		return true;
	}

} // namespace ml
