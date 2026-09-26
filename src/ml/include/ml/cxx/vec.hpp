#ifndef ML_VEC_HPP
#define ML_VEC_HPP

#include <ml/mem.h>
#include <ml/types.h>

#include <ml/cxx/allocator.hpp>
#include <new>

namespace ml {
	/// A vector/dynamic array.
	template <class T, class Alloc = StdAllocator<T> >
	struct Vec {
		typedef T* Pointer;
		typedef T& Reference;
		typedef const T& ConstReference;
		typedef usize SizeType;

		Vec() {
			pMemory = nil(T*);
			uiCapacity = 0;
			uiSize = 0;
		}

		Vec(const Vec& vec) {
			resize(vec.uiSize);
			for(SizeType i = 0; i < vec.uiSize; ++i)
				new(&this->pMemory[i]) T(vec.pMemory[i]);
		}

		Vec& operator=(const Vec& vec) {
			if(&vec == this) {
				return *this;
			}

			resize(vec.uiSize);
			for(SizeType i = 0; i < vec.uiSize; ++i)
				new(&this->pMemory[i]) T(vec.pMemory[i]);

			return *this;
		}

		~Vec() {
			clear();
		}

		void reserve(SizeType newCapacity) {
			if(newCapacity < uiCapacity) {
				// use resize() to downsize
				return;
			}

			Pointer pOldMemory = pMemory;
			SizeType oldCapacity = uiCapacity;

			pMemory = Alloc::allocateArray(newCapacity);

			if(pOldMemory) {
				// Initialize previous elements via copy-construction
				for(SizeType i = 0; i < oldCapacity; ++i)
					new(&pMemory[i]) T(pOldMemory[i]);

				// Initalize newly reserved elements with default construction
				for(SizeType i = oldCapacity; i < newCapacity; ++i)
					new(&pMemory[i]) T();

				destroy(pOldMemory, oldCapacity);
			} else {
				for(SizeType i = 0; i < newCapacity; ++i)
					new(&pMemory[i]) T();
			}

			uiCapacity = newCapacity;
		}

		void resize(SizeType uiNewSize) {
			if(uiNewSize > uiCapacity) {
				reserve(uiNewSize + 4);
			} else {
				// End lifetime of objects we no longer need.
				destruct(pMemory + uiNewSize, uiSize);
			}

			uiSize = uiNewSize;
		}

		void pushBack(ConstReference item) {
			resize(uiSize + 1);
			pMemory[uiSize - 1] = item;
		}

		void popBack() {
			if(uiSize == 0)
				return;
			resize(uiSize - 1);
		}

		void clear() {
			if(uiCapacity != 0) {
				destroy(pMemory, uiCapacity);
			}

			// Reset state so we seem like a default-constructed
			// Vec instance.
			uiCapacity = 0;
			uiSize = 0;
		}

		Reference operator[](SizeType index) {
			return pMemory[index];
		}

		ConstReference operator[](SizeType index) const {
			return pMemory[index];
		}

		Reference back() {
			return pMemory[uiSize - 1];
		}

		SizeType capacity() const {
			return uiCapacity;
		}

		SizeType size() const {
			return uiSize;
		}

	   private:
		/// Shared destroy helper for clearing out memory. if the
		/// bFree template arg is true, then this will deallocate and nil the pointer
		/// entirely.
		void destroy(T* pMemToDestroy, SizeType len) {
			Alloc::deallocate(pMemToDestroy);
			pMemToDestroy = nil(T*);
		}

		void destruct(T* pMemToDestruct, SizeType len) {
			for(SizeType i = 0; i < len; ++i)
				pMemToDestruct[i].~T();
		}

		T* pMemory;
		SizeType uiCapacity; // In elements of T
		SizeType uiSize;	 // Actual size.
	};
} // namespace ml

#endif
