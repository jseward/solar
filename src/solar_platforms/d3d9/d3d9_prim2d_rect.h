#pragma once

#include "solar/math/vec2.h"
#include "solar/colors/color.h"
#include "solar/rendering/textures/simple_rect_uvs.h"

namespace solar {

	struct d3d9_prim2d_rect {
	public:
		static const int VERTEX_COUNT = 4;
		static const int INDEX_COUNT = 6;

	public:
		vec2 _top_left;
		vec2 _top_right;
		vec2 _bottom_right;
		vec2 _bottom_left;
		color _color;
		simple_rect_uvs _uvs;

	public:
		d3d9_prim2d_rect(
			const vec2& top_left, 
			const vec2& top_right, 
			const vec2& bottom_right, 
			const vec2& bottom_left, 
			const color& color, 
			const simple_rect_uvs& uvs)

			: _top_left(top_left)
			, _top_right(top_right)
			, _bottom_right(bottom_right)
			, _bottom_left(bottom_left)
			, _color(color)
			, _uvs(uvs) {
		}
	};

}