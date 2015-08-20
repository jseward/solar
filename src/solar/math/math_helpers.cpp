#include "math_helpers.h"

#include "solar/utility/assert.h"

namespace solar {

	float sqrt(float x) {
		errno = 0;
		const float sqrt_x = ::sqrtf(x);
		ASSERT(errno == 0);
		return sqrt_x;
	}

	float abs(float x) {
		return ::fabsf(x);
	}

	float cos(deg d) {
		return ::cosf(d.to_rad());
	}

	float sin(deg d) {
		return ::sinf(d.to_rad());
	}

	float tan(deg d) {
		return ::tanf(d.to_rad());
	}

	bool is_approx(float x, float y, float tolerance) {
		return (abs(x - y) <= tolerance);
	}

	float constrain(float lower_bound, float upper_bound, float x) {
		ASSERT(lower_bound <= upper_bound);
		if (x < lower_bound) {
			return lower_bound;
		}
		else if (x > upper_bound) {
			return upper_bound;
		}
		return x;
	}
}