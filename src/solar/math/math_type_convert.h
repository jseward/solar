#pragma once

#include "solar/math/point.h"
#include "solar/math/pointf.h"
#include "solar/math/rect.h"
#include "solar/math/rectf.h"
#include "solar/math/vec2.h"
#include "solar/utility/type_convert.h"

namespace solar {

	inline vec2 point_to_vec2(const point& p) {
		return vec2(
			int_to_float(p._x), 
			int_to_float(p._y));
	}

	inline vec2 pointf_to_vec2(const pointf& p) {
		return vec2(p._x, p._y);
	}

	inline rect rectf_to_rect(const rectf& r) {
		return rect(
			float_to_int(r.get_left()),
			float_to_int(r.get_top()),
			float_to_int(r.get_right()),
			float_to_int(r.get_bottom()));
	}

	inline rectf rect_to_rectf(const rect& r) {
		return rectf(
			int_to_float(r.get_left()),
			int_to_float(r.get_top()),
			int_to_float(r.get_right()),
			int_to_float(r.get_bottom()));
	}

}