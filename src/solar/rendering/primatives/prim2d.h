#pragma once

#include <vector>
#include "solar/math/rect.h"
#include "solar/math/vec2.h"
#include "solar/rendering/color.h"
#include "solar/rendering/textures/simple_rect_uvs.h"
#include "prim2d_world_projection_buffer.h"

namespace solar {

	class shader;
	class texture;
	class viewport;
	class camera;

	class prim2d {
	private:
		const unsigned int DEFAULT_CIRCLE_SEGMENT_COUNT = 16;

	private:
		prim2d_world_projection_buffer _world_projection_buffer;

	public:
		virtual void begin_rendering(const rect& viewport_area) = 0;
		virtual void begin_rendering(const rect& viewport_area, shader& shader) = 0;
		virtual void end_rendering() = 0;
		virtual void set_shader(shader& shader) = 0;
		virtual void set_texture(texture& texture) = 0;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) = 0;
		virtual void render_tri(const vec2& v0, const vec2& v1, const vec2& v2, const color& color) = 0;
		virtual void render_indexed_tris(const vec2* vertices, unsigned int vertex_count, const unsigned short* indices, unsigned int index_count, const color& color) = 0;

	public:
		void render_rect(const rect& area, const color& color);
		void render_rect(const rect& area, const color& color, const simple_rect_uvs& uvs);
		void render_circle(const vec2& center, float radius, const color& color);
		void render_circle(const vec2& center, float radius, const color& color, unsigned int segment_count);
		void render_polygon(const vec2* points, unsigned int point_count, const color& color);

		void render_world_polygon(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color);
		void render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color);
		void render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color, unsigned int segment_count);
		void render_world_indexed_tris(const viewport& viewport, const camera& camera, const vec3* vertices, unsigned int vertex_count, const unsigned short* indices, unsigned int index_count, const color& color);
	};

}