#ifndef ML_CXX_HASHMAP_HPP
#define ML_CXX_HASHMAP_HPP

#include <ml/cxx/allocator.hpp>
#include <ml/cxx/hash.hpp>
#include <ml/cxx/string.hpp>
#include <ml/cxx/vec.hpp>

namespace ml {

	/// A basic hash map data structure.
	template <class K, class V, template <class T> class Alloc = StdAllocator>
	class HashMap {
		struct Node {
			K key;
			V value;
			Node* pNext; // Might be nil(Node*)
		};

		Node** pNodes;

	   public:
		typedef Hash<K> Hasher;

	   private:
		Node* findNodeForKey(const K& key) {
		}

	   public:
		/// Returns true if the key exists
		bool has(const K& key) {
		}

		V& operator[](const K& key) {
		}
	};

} // namespace ml

#endif
