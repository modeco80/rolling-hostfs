#ifndef ML_CXX_FREELIST_HPP
#define ML_CXX_FREELIST_HPP

#include <ml/mem.h>
#include <ml/string.h>

#include <ml/cxx/bitset.hpp>

namespace ml {

	/// Implements a free list of objects.
	/// NOTE: Only safe for POD types currently!!!!!!!!!
	template <class T, u32 NumItems>
	class FreeList {
		T* pItems;
		BitSet<NumItems> usedSet;

	   public:
		FreeList() {
			pItems = (T*)mlMalloc(NumItems * sizeof(T));
		}

		~FreeList() {
			mlFree(pItems);
			usedSet.reset();
		}

		T* allocate() {
			for(u32 i = 0; i < NumItems; ++i) {
				if(!usedSet[i]) {
					memset(&pItems[i], 0, sizeof(T));
					usedSet.set(i);
					return &pItems[i];
				}
			}

			return nil(T*);
		}

		void free(T* item) {
			// FIXME: This currently assumes the user isn't an idiot and is actually
			// passing a pointer to one of our items. We probably can fix this by asserting
			// (for debug) or ignoring wild indicies (release)? BitSet<N> can deal with out of bounds
			// indicies by just returning false on release, so it should be fine...
			const u32 index = (item - pItems);
			if(usedSet[index]) {
				usedSet.unset(index);
			}
		}

		u32 usedSlotCount() const {
			return usedSet.popCount();
		}
	};

} // namespace ml

#endif
