#pragma once

#include "deg.h"

namespace solar {

	extern float sqrt(float x);
	extern float abs(float x);

	extern float cos(deg d);
	extern float sin(deg d);
	extern float tan(deg d);

	extern bool is_approx(float x, float y, float tolerance);
	extern float constrain(float lower_bound, float upper_bound, float x);
}