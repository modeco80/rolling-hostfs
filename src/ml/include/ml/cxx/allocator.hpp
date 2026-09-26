#ifndef ML_ALLOCATOR_HPP
#define ML_ALLOCATOR_HPP

#include <ml/mem.h>

namespace ml {

	/// The standard allocator. Used by most cxx containers by default.
	template <class T>
	struct StdAllocator {
		template <class U>
		struct Rebind {
			typedef StdAllocator<U> Type;
		};

		static T* allocateArray(usize count) {
			return (T*)mlCalloc(count, sizeof(T));
		}

		static T* allocateOne() {
			return (T*)mlCalloc(1, sizeof(T));
		}

		static void deallocate(T* pObj) {
			if(pObj) {
				mlFree(pObj);
			}
		}
	};
} // namespace ml

#endif
