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

	inline uint8_t int32_to_uint8(int32_t v) {
		ASSERT(v >= 0);
		ASSERT(v <= std::numeric_limits<uint8_t>::max());
		return static_cast<uint8_t>(v);
	}

	inline int32_t uint32_to_int32(uint32_t v) {
		ASSERT(v <= static_cast<uint32_t>(std::numeric_limits<int32_t>::max()));
		return static_cast<int32_t>(v);
	}

	//alias for when explicit int size not necessary in client code
	inline int uint_to_int(unsigned int v) {
		return uint32_to_int32(v);
	}

	inline uint8_t uint32_to_uint8(uint32_t v) {
		ASSERT(v <= std::numeric_limits<uint8_t>::max());
		return static_cast<uint8_t>(v);
	}

	inline uint16_t uint32_to_uint16(uint32_t v) {
		ASSERT(v <= std::numeric_limits<uint16_t>::max());
		return static_cast<uint16_t>(v);
	}

}
