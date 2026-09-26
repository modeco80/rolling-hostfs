#ifndef ML_CXX_SINGLEGON_HPP
#define ML_CXX_SINGLEGON_HPP

#include <ml/types.h>

#include <new>

namespace ml {

	/// A singleton which allocates the memory for the object on the heap.
	template <class T>
	class PtrSingleton {
		T* instance;

	   public:
		PtrSingleton() {
		}
		~PtrSingleton() {
			destroy();
		}

		T* getPtr() {
			if(instance == nil(T*)) {
				instance = new T();
			}

			return instance;
		}

		T& get() {
			return *getPtr();
		}

		void destroy() {
			if(instance) {
				delete instance;
				instance = nil(T*);
			}
		}
	};

	/// A singleton which allocates its data statically, without any heap allocations.
	/// You should probably only use this spairingly for small classes.
	template <class T>
	class StaticSingleton {
		struct {
			bool init;
			u8 instanceData[sizeof(T)];
		} initData;

	   public:
		StaticSingleton() {
			initData.init = false;
		}

		~StaticSingleton() {
			destroy();
		}

		T* getPtr() {
			if(!initData.init) {
				initData.init = true;
				return *new(&initData.instanceData[0]) T();
			}

			return *(T*)&initData.instanceData[0];
		}

		T& get() {
			return *getPtr();
		}

		void destroy() {
			get().~T();
			initData.init = false;
		}
	};

} // namespace ml
#endif
