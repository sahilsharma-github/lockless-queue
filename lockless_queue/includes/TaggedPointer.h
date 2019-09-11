#ifndef TAGGED_POINTER_H


#include <cassert>
#include <stdint.h>

template <typename T, int wordsize>
class TaggedPointer {
private:
	static_assert(
		wordsize != 0 && ((wordsize& (wordsize - 1)) == 0),
		"Word size parameter must be a multiple of two"
		);

	// for 8 byte alignment tagMask = wordsize - 1 = 8 - 1 = 7 = 0b111
	// i.e. the lowest three bits are set, which is where the tag is stored
	static const intptr_t tagMask = wordsize - 1;

	// pointerMask is the exact contrary: 0b...11111000
	// i.e. all bits apart from the three lowest are set, which is where the pointer is stored
	static const intptr_t pointerMask = ~tagMask;

	// save us some reinterpret_casts with a union
	union {
		T* asPointer;
		intptr_t asBits;
	};

public:
	inline TaggedPointer(T* pointer = 0, int tag = 0) {
		set(pointer, tag);
	}
	TaggedPointer() {
		//set(pointer, tag);
	}

	inline void set(T* pointer, int tag = 0) {
		// make sure that the pointer really is aligned
		assert((reinterpret_cast<intptr_t>(pointer) & tagMask) == 0);
		// make sure that the tag isn't too large
		assert((tag & pointerMask) == 0);

		asPointer = pointer;
		asBits |= tag;
	}

	inline T * getPointer() const {
		return reinterpret_cast<T*>(asBits & pointerMask);
	}
	inline int getTag() const {
		return asBits & tagMask;
	}
};


#endif

