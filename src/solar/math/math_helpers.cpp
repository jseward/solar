#include "math_helpers.h"

#include "solar/utility/verify.h"
#include "solar/math/math_constants.h"

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

	float cos(float radians) {
		return ::cosf(radians);
	}

	float sin(float radians) {
		return ::sinf(radians);
	}

	float tan(float radians) {
		return ::tanf(radians);
	}

	bool is_approx(float x, float y, float tolerance) {
		return (abs(x - y) <= tolerance);
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

	float lerp(float v0, float v1, float t) {
		return (v0 + ((v1 - v0) * t));
	}

	float calculate_oscillate_t(float current_time, float oscillation_time_period) {
		float t = 0.f;
		IF_VERIFY(oscillation_time_period > 0.f) {
			t = sin((std::fmodf(current_time, oscillation_time_period) / oscillation_time_period) * TWO_PI);
			//sin is in range -1 : +1, want 0 : +1
			t = (t + 1.f) / 2.f;
		}
		return t;
	}

}