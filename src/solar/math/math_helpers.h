#pragma once

#include "deg.h"
#include "vec2.h"

namespace solar {

	extern float sqrt(float x);
	extern float abs(float x);

	extern float cos(deg d);
	extern float sin(deg d);
	extern float tan(deg d);

	extern bool is_approx(float x, float y, float tolerance);
	extern float constrain(float lower_bound, float upper_bound, float x);
	extern bool is_within_inclusive(float lower_bound, float upper_bound, float x);
	extern bool is_within_exclusive(float lower_bound, float upper_bound, float x);

	//bounds:0-9 and x:11 -> 2
	//bounds:0-9 and x:-4 -> 6
	extern int int_wrap(int lower, int upper, int x);

	extern vec2 calculate_catmull_rom(const vec2& c0, const vec2& c1, const vec2& c2, const vec2& c3, float t);

	extern float lerp(float v0, float v1, float t);

	//returns value between 0-1 that is oscillating within time periord
	extern float calculate_oscillate_t(float current_time, float oscillation_time_period);

}