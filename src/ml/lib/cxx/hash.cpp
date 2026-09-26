#include <ml/cxx/hash.hpp>

namespace ml {

	HashResult hashBuffer(const u8* pBuffer, usize size) {
		// Do an ELF hash.
		u32 h = 0;
		u32 high = 0;
		while(size) {
			h = (h << 4) + *pBuffer++;
			if((high = h & 0xF0000000))
				h ^= high >> 24;
			h &= ~high;
			size--;
		}
		return h;
	}

} // namespace ml
