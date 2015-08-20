#pragma once

#include <limits>
#include "solar/utility/assert.h"

namespace solar {

	inline int float_to_int(float v) {
		return static_cast<int>(v);
	}

	inline float int_to_float(int v) {
		return static_cast<float>(v);
	}

	inline float uint_to_float(unsigned int v) {
		return static_cast<float>(v);
	}

	inline float double_to_float(double v) {
		return static_cast<float>(v);
	}

	inline unsigned short int_to_ushort(int v) {
		ASSERT(v >= 0);
		ASSERT(v <= std::numeric_limits<unsigned short>::max());
		return static_cast<unsigned short>(v);
	}

	inline unsigned short uint_to_ushort(unsigned int v) {
		ASSERT(v >= 0);
		ASSERT(v <= std::numeric_limits<unsigned short>::max());
		return static_cast<unsigned short>(v);
	}

	inline unsigned char ushort_to_uchar(unsigned short v) {
		ASSERT(v <= std::numeric_limits<unsigned char>::max());
		return static_cast<unsigned char>(v);
	}
}