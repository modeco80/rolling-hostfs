#ifndef ML_CXX_FIXEDVEC_HPP
#define ML_CXX_FIXEDVEC_HPP

#include <ml/types.h>

namespace ml {
	/// A fixed version of ml::Vec<>. Uses static storage, and can only grow to
	/// a maximum fixed size.
	template <class T, u32 N>
	class FixedVec {
		T storage[N];
		u32 vecSize;

	   public:
		FixedVec() {
			vecSize = 0;
		}

		bool push(const T& item) {
			if(vecSize + 1 > N) {
				return false;
			}
			storage[vecSize++] = item;
			return true;
		}

		T pop() {
			mlASSERT(!empty());
			T item = storage[vecSize];
			vecSize--;
			return item;
		}

		T& peekTop() {
			mlASSERT(!empty());
			return storage[vecSize];
		}

		T& peekBottom() {
			mlASSERT(!empty());
			return storage[0];
		}

		template <class Predicate>
		u32 find(Predicate& predicate) {
			for(u32 i = 0; i < vecSize; ++i) {
				if(predicate.operator()(storage[i])) {
					return i;
				}
			}
			return -1;
		}

		template <class Predicate>
		void forEach(Predicate& predicate) {
			for(u32 i = 0; i < vecSize; ++i)
				if(!predicate.operator()(storage[i]))
					return;
		}

		void remove(u32 pos) {
			for(u32 i = pos + 1; i < vecSize; ++i)
				storage[i - 1] = storage[i];
			vecSize--;
		}

		void clear() {
			vecSize = 0;
		}

		bool empty() {
			return vecSize == 0;
		}

		u32 size() const {
			return vecSize;
		}

		T& operator[](usize index) {
			return storage[index];
		}

		const T& operator[](usize index) const {
			return storage[index];
		}
	};
} // namespace ml

#endif
