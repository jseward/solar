#include "simple_rect_uvs.h"
#include "solar/math/size.h"
#include "solar/math/rect.h"

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

		if (texture_size.get_width() > 0 && texture_size.get_height() > 0) {
			_left_u = static_cast<float>(texel_box.get_left()) / static_cast<float>(texture_size.get_width());
			_top_v = static_cast<float>(texel_box.get_top()) / static_cast<float>(texture_size.get_height());
			_right_u = static_cast<float>(texel_box.get_right()) / static_cast<float>(texture_size.get_width());
			_bottom_v = static_cast<float>(texel_box.get_bottom()) / static_cast<float>(texture_size.get_height());
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