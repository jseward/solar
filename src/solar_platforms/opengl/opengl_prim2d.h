#pragma once

#include <vector>
#include "solar/rendering/primatives/prim2d.h"
#include "solar/utility/property.h"
#include "solar/containers/dynamic_vector.h"
#include "opengl_prim2d_vertex.h"

namespace solar {

	class opengl_context;
	class opengl_shader_program;
	class opengl_shader_program_factory;

	struct opengl_prim2d_setup_params {
		MAKE_PROPERTY(opengl_prim2d_setup_params, unsigned int, max_buffered_rect_count, 600);
		MAKE_PROPERTY(opengl_prim2d_setup_params, unsigned int, max_buffered_tri_count, 600);
	};

	class opengl_prim2d : public prim2d {
	private:
		static const int RECT_VERTEX_COUNT;
		static const int RECT_INDEX_COUNT;
		static const int TRI_VERTEX_COUNT;
		static const int TRI_INDEX_COUNT;

	private:
		opengl_context& _context;
		opengl_shader_program_factory& _factory;

		opengl_prim2d_setup_params _setup_params;
		unsigned int _max_buffered_vertices;
		dynamic_vector<opengl_prim2d_vertex> _buffered_vertices;
		unsigned int _max_buffered_indices;
		dynamic_vector<uint16_t> _buffered_indices;

		opengl_shader_program* _default_shader_program;
		render_state_block* _default_rs_block;
		
		bool _is_rendering;
		shader_program* _rendering_shader_program;

	public:
		opengl_prim2d(opengl_context& context, opengl_shader_program_factory& factory);
		virtual ~opengl_prim2d();

		void setup(const opengl_prim2d_setup_params& params);
		void teardown();
		
		virtual void begin_rendering(const rect& viewport_area) override;
		virtual void begin_rendering(const rect& viewport_area, shader_program& shader_program, render_state_block* rs_block) override;
		virtual void end_rendering() override;
		virtual void set_shader_program(shader_program& shader_program) override;
		virtual void set_texture(texture* texture) override;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) override;
		virtual void render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) override;

	private:
		void flush_and_submit_buffer();
	};

}
