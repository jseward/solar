#pragma once

#include "solar/math/rect.h"
#include "solar/math/vec2.h"
#include "solar/rendering/color.h"
#include "solar/rendering/textures/simple_rect_uvs.h"

namespace solar {

	class shader;
	class texture;

	class prim2d {
	public:
		virtual void begin_rendering(const rect& viewport_area, shader& shader) = 0;
		virtual void end_rendering() = 0;
		virtual void set_shader(shader& shader) = 0;
		virtual void set_texture(texture& texture) = 0;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) = 0;

	public:
		void render_rect(const rect& area, const color& color);
		void render_rect(const rect& area, const color& color, const simple_rect_uvs& uvs);
	};

}