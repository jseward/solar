#pragma once

#include <vector>
#include "solar/math/vec2.h"
#include "solar/math/vec3.h"
#include "solar/rendering/color.h"
#include "prim2d_world_projection_buffer.h"

namespace solar {

	class viewport;
	class camera;

	class prim2d_lines {
	private:
		const unsigned int DEFAULT_CIRCLE_SEGMENT_COUNT = 16;

	private:
		prim2d_world_projection_buffer _world_projection_buffer;
		std::vector<vec2> _render_buffer;

	public:
		virtual bool is_rendering() const = 0;
		virtual void begin_rendering(float line_width) = 0;
		virtual void end_rendering() = 0;
		virtual void render_segments(const vec2* points, unsigned int point_count, const color& color) = 0;

		void render_segment(const vec2& p0, const vec2& p1, const color& color);
		void render_segments_looped(const vec2* points, unsigned int point_count, const color& color);
		void render_circle(const vec2& center, float radius, const color& color);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad);
		void render_circle(const vec2& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count);
		void render_rect(const rect& area, const color& color);

		void render_world_segment(const viewport& viewport, const camera& camera, const vec3& p0, const vec3& p1, const color& color);
		void render_world_segments(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color);
		void render_world_segments_looped(const viewport& viewport, const camera& camera, const vec2* points, unsigned int point_count, const color& color);
		void render_world_segments_looped(const viewport& viewport, const camera& camera, const vec3* points, unsigned int point_count, const color& color);
		void render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color);
		void render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color, float begin_rad, float end_rad);
		void render_world_circle(const viewport& viewport, const camera& camera, const vec3& center, float radius, const color& color, float begin_rad, float end_rad, unsigned int segment_count);
	};

}