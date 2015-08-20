#pragma once

#include "d3d9_headers.h"
#include "solar/math/vec2.h"
#include "solar/rendering/color.h"
#include "solar/rendering/textures/uv.h"

namespace solar {

	struct d3d9_prim2d_vertex {
	public:
		static const DWORD FVF;

	public:
		float _sx;
		float _sy;
		float _sz;
		float _rhw;
		D3DCOLOR _color;
		D3DCOLOR _align32;
		float _tu;
		float _tv;

	public:
		void set(const vec2& pos, const color& color, const uv& uv);
	};

}