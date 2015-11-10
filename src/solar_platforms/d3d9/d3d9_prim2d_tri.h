#pragma once

#include "solar/math/vec2.h"
#include "solar/colors/color.h"
#include "solar/rendering/textures/uv.h"

namespace solar {

	struct d3d9_prim2d_tri {
	public:
		static const int VERTEX_COUNT = 3;
		static const int INDEX_COUNT = 3;

	public:
		vec2 _p0;
		uv _uv0;
		vec2 _p1;
		uv _uv1;
		vec2 _p2;
		uv _uv2;
		color _color;

	public:
		d3d9_prim2d_tri(
			const vec2& p0, 
			const uv& uv0,
			const vec2& p1,
			const uv& uv1,
			const vec2& p2,
			const uv& uv2,
			const color& color)

			: _p0(p0)
			, _uv0(uv0)
			, _p1(p1)
			, _uv1(uv1)
			, _p2(p2)
			, _uv2(uv2)
			, _color(color) {
		}
	};

}