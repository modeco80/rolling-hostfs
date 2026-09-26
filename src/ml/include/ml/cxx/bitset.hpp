#ifndef ML_CXX_BITSET_HPP
#define ML_CXX_BITSET_HPP

#include <ml/types.h>

namespace ml {

	namespace dragons {

		/// A tiny bitset which supports a various range of sizes.
		/// Unlike later STL std::bitset, there's no proxy objects,
		/// none of that faff in what's effectively embedded code.
		template <u32 NrBits>
		class BitSet {
			enum {
				ArraySize = (NrBits / (sizeof(u32) * 8)) ? (NrBits / (sizeof(u32) * 8)) : 1
			};

			u32 bitArray[ArraySize];

#define BIT_SIZE (sizeof(u32) * 8)
#define BIT_ARRAY_INDEX(index) (u32)(index / BIT_SIZE)
#define MAKE_MASK(index) ((u32)1 << (index % BIT_SIZE))

		   public:
			BitSet() {
				reset();
			}

			void reset() {
				// Avoids reliance on ml/mem header. Kinda stupid but
				// what are you gonna do.
				for(u32 i = 0; i < ArraySize; ++i)
					bitArray[i] = 0;
			}

			void set(u32 index) {
				if(index > NrBits)
					return;
				bitArray[BIT_ARRAY_INDEX(index)] |= MAKE_MASK(index);
			}

			void unset(u32 index) {
				if(index > NrBits)
					return;
				bitArray[BIT_ARRAY_INDEX(index)] &= ~MAKE_MASK(index);
			}

			bool test(u32 index) const {
				if(index > NrBits)
					return false;
				const u32 MASK = MAKE_MASK(index);
				return (bitArray[BIT_ARRAY_INDEX(index)] & MASK) == MASK;
			}

			bool operator[](u32 index) const {
				return test(index);
			}

			// helpers
			u32 popCount() const {
				u32 count = 0;
				for(u32 i = 0; i < NrBits; ++i) {
					// manually inlining test() to make sure compiler isn't stupid
					const u32 MASK = MAKE_MASK(i);
					if((bitArray[BIT_ARRAY_INDEX(i)] & MASK) == MASK)
						count++;
				}

				return count;
			}

#undef BIT_SIZE
#undef BIT_ARRAY_INDEX
#undef MAKE_MASK
		};

	} // namespace dragons

	using dragons::BitSet;
} // namespace ml

#endif
