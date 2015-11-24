#pragma once

#include <bgfx/bgfx.h>
#include "solar/math/vec2.h"
#include "solar/colors/color.h"
#include "solar/rendering/textures/uv.h"

namespace solar {

	class bgfx_prim2d_vertex {
	public:
		float _sx;
		float _sy;
		float _sz;
		uint32_t _color;
		float _tu;
		float _tv;

	public:
		bgfx_prim2d_vertex()
			: _sx(0.f)
			, _sy(0.f)
			, _sz(0.f)
			, _color(0)
			, _tu(0.f)
			, _tv(0.f) {}

		bgfx_prim2d_vertex(const vec2& screen_pos, const color& color, const uv& uv)
			: _sx(screen_pos._x)
			, _sy(screen_pos._y)
			, _sz(0.f)
			, _color(color.to_argb32())
			, _tu(uv._u)
			, _tv(uv._v) {}

	public:
		static void build_decl(bgfx::VertexDecl& decl, bgfx::RendererType::Enum renderer_type);
	};

}