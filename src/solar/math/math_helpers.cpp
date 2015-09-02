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

	int int_wrap(int lower, int upper, int x) {
		int range = upper - lower + 1;
		ASSERT(range > 0);

		int wrapped_x = x % range;
		if (wrapped_x < lower) {
			wrapped_x += range;
		}
		return wrapped_x;
	}

	vec2 calculate_catmull_rom(const vec2& c0, const vec2& c1, const vec2& c2, const vec2& c3, float t) {
		const float t2 = t * t;
		const float t3 = t2 * t;
		const vec2 v0 = c0 * (-t3 + (2.f * t2) - t);
		const vec2 v1 = c1 * ((3.f * t3) - (5.f * t2) + 2.f);
		const vec2 v2 = c2 * ((-3.f * t3) + (4.f * t2) + t);
		const vec2 v3 = c3 * (t3 - t2);
		return ((v0 + v1 + v2 + v3) / 2.f);
	}
}