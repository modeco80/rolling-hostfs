#ifndef ML_CXX_HASH_HPP
#define ML_CXX_HASH_HPP

#include <ml/types.h>

namespace ml {

	/// The result of a mlHash<T>::hash() or mlHashBuffer call
	typedef u32 HashResult;

	/// Hash a buffer.
	HashResult hashBuffer(const u8* pBuffer, usize size);

	/// Typed hashing trait class.
	template <class T>
	class Hash {};

/// This.. interesting usage of the C preprocessor automatically generates primitive mlHash<T>
/// specializations. It's pretty nasty, but it also works. Which is the best kind :)
#define X(T)                                                                  \
	template <>                                                               \
	class Hash<T> {                                                           \
	   public:                                                                \
		static HashResult hash(const T item) {                                \
			return hashBuffer(reinterpret_cast<const u8*>(&item), sizeof(T)); \
		}                                                                     \
	};
	ML_PRIMITIVE_TYPE_LIST()
#undef X

} // namespace ml

#endif
