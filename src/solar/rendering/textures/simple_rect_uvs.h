#pragma once

#include "uv.h"

namespace solar {

	class size;
	class rect;

	class simple_rect_uvs {
	private:
		float _left_u;
		float _top_v;
		float _right_u;
		float _bottom_v;

	public:
		simple_rect_uvs();
		simple_rect_uvs(const size& texture_size, const rect& texel_box);

		float get_left_u() const;
		float get_top_v() const;
		float get_right_u() const;
		float got_bottom_v() const;

		uv get_top_left() const;
		uv get_top_right() const;
		uv get_bottom_right() const;
		uv get_bottom_left() const;
	};

}