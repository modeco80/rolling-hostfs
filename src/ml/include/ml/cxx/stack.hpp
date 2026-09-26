#ifndef ML_CXX_STACK_HPP
#define ML_CXX_STACK_HPP

#include <ml/cxx/allocator.hpp>
#include <ml/cxx/vec.hpp>

// TODO: Having this able to use multiple containers instead of just mlVec<> would be nice,
// but for now it's probably fine.

namespace ml {

	/// A stack data structure.
	template <class T, class Alloc = mlStdAllocator<T>>
	class Stack {
		typedef T& Reference;
		typedef const T& ConstReference;
		Vec<T, Alloc> items;
		u32 top;

	   public:
		Stack() {
			top = 0;
		}

		void push(ConstReference item) {
			items.pushBack(item);
			top++;
		}

		T pop() {
			if(empty()) {
				// TODO: We probably should actually have a crash thing
				while(true);
			}

			// Pop the item off the stack
			T item = items[top];
			items.popBack();
			top--;
			return item;
		}

		Reference peekTop() {
			if(empty()) {
				while(true);
			}
			return items[top];
		}

		void clear() {
			items.clear();
			top = 0;
		}

		bool empty() {
			return top == 0;
		}

		u32 size() const {
			// guess you could do items.size() too,
			// but I'd logically want size() to be
			// how large the stack is. which top communicates
			return top;
		}
	};
} // namespace ml

#endif
