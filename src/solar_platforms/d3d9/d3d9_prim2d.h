#pragma once

#include <vector>
#include "solar/rendering/primatives/prim2d.h"
#include "solar/utility/property.h"
#include "d3d9_context.h"
#include "d3d9_device_event_handler.h"
#include "d3d9_dynamic_vertex_buffer.h"
#include "d3d9_dynamic_index_buffer.h"
#include "d3d9_solid_color_texture.h"
#include "d3d9_prim2d_rect.h"
#include "d3d9_prim2d_tri.h"
#include "d3d9_prim2d_vertex.h"

namespace solar {

	struct d3d9_prim2d_setup_params {
		MAKE_PROPERTY(d3d9_prim2d_setup_params, unsigned int, max_buffered_rect_count, 600);
		MAKE_PROPERTY(d3d9_prim2d_setup_params, unsigned int, max_buffered_tri_count, 600);
	};

	class d3d9_shader_program;
	class d3d9_shader_program_factory;

	class d3d9_prim2d 
		: public prim2d
		, public d3d9_device_event_handler {

	private:
		struct shader_param_names {
			static const char* TEXTURE;
		};

	private:
		d3d9_context& _context;
		d3d9_shader_program_factory& _shader_program_factory;
		d3d9_prim2d_setup_params _setup_params;
		std::vector<d3d9_prim2d_rect> _buffered_rects;
		std::vector<d3d9_prim2d_tri> _buffered_tris;
		d3d9_shader_program* _default_shader_program;
		render_state_block* _default_rs_block;
		d3d9_solid_color_texture _white_texture;
		bool _is_rendering;
		shader_program* _shader_program;
		texture* _texture;
		d3d9_dynamic_vertex_buffer<d3d9_prim2d_vertex> _vertex_buffer;
		d3d9_dynamic_index_buffer _index_buffer;

	public:
		d3d9_prim2d(d3d9_context& context, d3d9_shader_program_factory& shader_program_factory);
		virtual ~d3d9_prim2d();

		void setup(const d3d9_prim2d_setup_params& params);
		void teardown();
		
		virtual void begin_rendering(const rect& viewport_area) override;
		virtual void begin_rendering(const rect& viewport_area, shader_program& program, render_state_block* rs_block) override;
		virtual void end_rendering() override;
		virtual void set_shader_program(shader_program& program) override;
		virtual void set_texture(texture* texture) override;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) override;
		virtual void render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) override;

	private:
		void flush_all();
		void flush_rects();
		void flush_tris();

		struct lock_buffers_result {
			d3d9_prim2d_vertex* _vertices;
			WORD* _indices;
			int _vertices_begin;

		public:
			lock_buffers_result(d3d9_prim2d_vertex* vertices, WORD* indices, int vertices_begin)
				: _vertices(vertices)
				, _indices(indices)
				, _vertices_begin(vertices_begin) {
			}
		};

		lock_buffers_result lock_buffers_and_render_if_needed(int vertices_required, int indices_required);
		void unlock_buffers_and_render();

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}