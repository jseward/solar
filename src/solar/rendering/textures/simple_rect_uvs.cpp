#include "simple_rect_uvs.h"
#include "solar/math/size.h"
#include "solar/math/rect.h"
#include "solar/utility/type_convert.h"

namespace solar {

	simple_rect_uvs::simple_rect_uvs()
		: _left_u(0.f)
		, _top_v(0.f)
		, _right_u(1.f)
		, _bottom_v(1.f) {
	}

	simple_rect_uvs::simple_rect_uvs(const size& texture_size, const rect& texel_box)
		: _left_u(0.f)
		, _top_v(0.f)
		, _right_u(1.f)
		, _bottom_v(1.f) {

		if (texture_size._width > 0 && texture_size._height > 0) {
			_left_u = int_to_float(texel_box.get_left()) / int_to_float(texture_size._width);
			_top_v = int_to_float(texel_box.get_top()) / int_to_float(texture_size._height);
			_right_u = int_to_float(texel_box.get_right()) / int_to_float(texture_size._width);
			_bottom_v = int_to_float(texel_box.get_bottom()) / int_to_float(texture_size._height);
		}
	}

	float simple_rect_uvs::get_left_u() const {
		return _left_u;
	}

	float simple_rect_uvs::get_right_u() const {
		return _right_u;
	}

	float simple_rect_uvs::get_top_v() const {
		return _top_v;
	}

	float simple_rect_uvs::got_bottom_v() const {
		return _bottom_v;
	}

	uv simple_rect_uvs::get_top_left() const {
		return uv(_left_u, _top_v);
	}

	uv simple_rect_uvs::get_top_right() const {
		return uv(_right_u, _top_v);
	}

	uv simple_rect_uvs::get_bottom_right() const {
		return uv(_right_u, _bottom_v);
	}

	uv simple_rect_uvs::get_bottom_left() const {
		return uv(_left_u, _bottom_v);
	}

}