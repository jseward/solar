#include "d3d9_prim2d_rect.h"

namespace solar {

	d3d9_prim2d_rect::d3d9_prim2d_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs)
		: _top_left(top_left)
		, _top_right(top_right)
		, _bottom_right(bottom_right)
		, _bottom_left(bottom_left)
		, _color(color) 
		, _uvs(uvs) {
	}

}