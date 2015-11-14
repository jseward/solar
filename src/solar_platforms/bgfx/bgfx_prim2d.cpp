#include "bgfx_prim2d.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_prim2d::bgfx_prim2d() {
	}

	bgfx_prim2d::~bgfx_prim2d() {
	}

	void bgfx_prim2d::setup() {
	}

	void bgfx_prim2d::teardown() {
	}

	void bgfx_prim2d::begin_rendering(const rect& viewport_area) {
		//todo
		UNUSED_PARAMETER(viewport_area);
	}

	void bgfx_prim2d::begin_rendering(const rect& viewport_area, shader_program& shader_program, render_state_block* rs_block) {
		//todo
		UNUSED_PARAMETER(viewport_area);
		UNUSED_PARAMETER(shader_program);
		UNUSED_PARAMETER(rs_block);
		ASSERT(false);
	}

	void bgfx_prim2d::end_rendering() {
		//todo
	}

	void bgfx_prim2d::set_shader_program(shader_program& shader_program) {
		//todo
		UNUSED_PARAMETER(shader_program);
	}

	void bgfx_prim2d::set_texture(texture& texture) {
		//todo
		UNUSED_PARAMETER(texture);
	}

	void bgfx_prim2d::render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) {
		//todo
		UNUSED_PARAMETER(top_left);
		UNUSED_PARAMETER(top_right);
		UNUSED_PARAMETER(bottom_right);
		UNUSED_PARAMETER(bottom_left);
		UNUSED_PARAMETER(color);
		UNUSED_PARAMETER(uvs);
	}

	void bgfx_prim2d::render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) {
		//todo
		UNUSED_PARAMETER(p0);
		UNUSED_PARAMETER(uv0);
		UNUSED_PARAMETER(p1);
		UNUSED_PARAMETER(uv1);
		UNUSED_PARAMETER(p2);
		UNUSED_PARAMETER(uv2);
		UNUSED_PARAMETER(color);
	}

}