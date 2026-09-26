#ifndef ML_CXX_STRING_HPP
#define ML_CXX_STRING_HPP

#include <ml/types.h>

#include <ml/cxx/hash.hpp>

namespace ml {

	/// A basic byte-string. Cxx doesn't care what you put in here, but
	/// we allocate an extra null terminator for C strings.
	class String {
		char* mem;
		u32 length;
		u32 capacity;

	   public:
		String();
		String(const char* str);
		String(const String&);
		~String();

		// Assignment operator overload
		String& operator=(const String&);
		String& operator=(const char*);

		// TODO:
		// operator+
		// operator+=

		/// Alternative assignment.
		void assign(const String& str);
		void assign(const char* str);

		/// Resize the string. May reallocate the string.
		void resize(u32 len);

		/// Adjusts/allocates capacity, but does not actually resize the string's length.
		/// Useful for pre-allocation.
		void reserve(u32 newCapacity);

		/// Clear the string data.
		void clear();

		/// Retrns true if the string is empty.
		bool empty() const;

		// inlines

		char& operator[](usize index) {
			return mem[index];
		}
		const char& operator[](usize index) const {
			return mem[index];
		}

		u32 getLength() const {
			return length;
		}
		u32 getCapacity() const {
			return capacity;
		}

		const char* cStr() const {
			return mem;
		}
		char* data() {
			return mem;
		}
	};

	/// Hash<T> specialization
	template <>
	class Hash<String> {
	   public:
		static HashResult hash(const String& str) {
			return hashBuffer(reinterpret_cast<const u8*>(str.cStr()), str.getLength());
		}
	};

} // namespace ml

#endif
