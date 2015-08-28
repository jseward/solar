#pragma once

#include "solar/math/vec2.h"
#include "solar/rendering/color.h"

namespace solar {

	struct d3d9_prim2d_tri {
	public:
		static const int VERTEX_COUNT = 3;
		static const int INDEX_COUNT = 3;

	public:
		vec2 _p0;
		vec2 _p1;
		vec2 _p2;
		color _color;

	public:
		d3d9_prim2d_tri(
			const vec2& p0, 
			const vec2& p1,
			const vec2& p2,
			const color& color)

			: _p0(p0)
			, _p1(p1)
			, _p2(p2)
			, _color(color) {
		}
	};

}