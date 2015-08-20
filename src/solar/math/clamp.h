#pragma once

#include "solar/utility/assert.h"

namespace solar {

	template<typename T> T clamp(T min, T max, T value) {
		ASSERT(min <= max);
		if (value < min) {
			return min;
		}
		else if (value > max) {
			return max;
		}
		return value;
	}

}