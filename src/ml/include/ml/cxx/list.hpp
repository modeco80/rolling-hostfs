#ifndef ML_LIST_HPP
#define ML_LIST_HPP

#include <ml/types.h>

#include <ml/cxx/allocator.hpp>
#include <new>

namespace ml {

	// A linked list.
	template <class T, class Alloc = StdAllocator<T>>
	struct List {
		struct Node {
			Node* pNext;
			T item;

			~Node() {
				if(pNext) {
					pNext->~Node();
					Alloc::deallocate(pNext);
				}
			}
		};

		List() {
			pHead = nil(Node*);
		}

		~List() {
			clear();
		}

		void pushBack(const T& value) {
			Node* pNode = new(Alloc::allocateOne()) Node();
			pNode->item = value;
			pNode->pNext = nil(Node*);

			if(pHead == nil(Node*)) {
				pHead = pNode;
				return;
			}

			Node* iter = pHead;
			while(iter) {
				iter = iter->pNext;
			}

			iter->pNext = pNode;
		}

		u32 size() const {
			Node* iter = pHead;
			u32 count = 0;
			while(iter) {
				count++;
				iter = iter->pNext;
			}

			return count;
		}

		void forEach(bool (*pcbCallback)(T* item, void* ctx), void* ctx) {
			Node* iter = pHead;
			while(iter) {
				if(!pcbCallback(&iter->item, ctx))
					break;
				iter = iter->pNext;
			}
		}

		void clear() {
			if(pHead != nil(Node*)) {
				pHead->~Node();
				Alloc::deallocate(pHead);
				pHead = nil(Node*);
			}
		}

		T& operator[](u32 index) {
			Node* iter = pHead;
			u32 count = 0;
			while(iter) {
				if(count++ == index)
					return iter->item;

				iter = iter->pNext;
			}

			while(true);
		}

	   private:
		Node* pHead;
	};

} // namespace ml
#endif
