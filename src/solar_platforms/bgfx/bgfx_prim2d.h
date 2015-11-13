#pragma once

#include "solar/rendering/primatives/prim2d.h"

namespace solar {

	class bgfx_prim2d : public prim2d {
	public:
		bgfx_prim2d();
		virtual ~bgfx_prim2d();

		void setup();
		void teardown();
		
		virtual void begin_rendering(const rect& viewport_area) override;
		virtual void begin_rendering(const rect& viewport_area, shader& shader, render_state_block* rs_block) override;
		virtual void end_rendering() override;
		virtual void set_shader(shader& shader) override;
		virtual void set_texture(texture& texture) override;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) override;
		virtual void render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) override;
	};

}