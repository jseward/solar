#pragma once

#include "vec2.h"
#include "solar/utility/assert.h"

namespace solar {

	extern float sqrt(float x);
	extern float abs(float x);

	extern float cos(float radians);
	extern float sin(float radians);
	extern float tan(float radians);

	extern bool is_approx(float x, float y, float tolerance);
	
	//bounds:0-9 and x:11 -> 2
	//bounds:0-9 and x:-4 -> 6
	extern int int_wrap(int lower, int upper, int x);

	extern vec2 calculate_catmull_rom(const vec2& c0, const vec2& c1, const vec2& c2, const vec2& c3, float t);

	extern float lerp(float v0, float v1, float t);

	//returns value between 0-1 that is oscillating within time periord
	extern float calculate_oscillate_t(float current_time, float oscillation_time_period);

	template<typename T> T constrain(T lower_bound, T upper_bound, T x) {
		ASSERT(lower_bound <= upper_bound);
		if (x < lower_bound) {
			return lower_bound;
		}
		else if (x > upper_bound) {
			return upper_bound;
		}
		return x;
	}

	template<typename T> bool is_within_inclusive(T lower_bound, T upper_bound, T x) {
		return (x >= lower_bound && x <= upper_bound);
	}

	template<typename T> bool is_within_exclusive(T lower_bound, T upper_bound, T x) {
		return (x > lower_bound && x < upper_bound);
	}

}