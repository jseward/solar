#pragma once

#include "solar/math/point.h"
#include "solar/math/vec2.h"
#include "solar/utility/type_convert.h"

namespace solar {

	inline vec2 point_to_vec2(const point& p) {
		return vec2(
			int_to_float(p._x), 
			int_to_float(p._y));
	}

}