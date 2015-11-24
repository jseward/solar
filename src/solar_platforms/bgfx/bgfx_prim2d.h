#pragma once

#include <vector>
#include <bgfx/bgfx.h>
#include "solar/rendering/primatives/prim2d.h"
#include "solar/utility/property.h"
#include "solar/containers/dynamic_vector.h"
#include "bgfx_prim2d_vertex.h"

namespace solar {

	class bgfx_context;
	class bgfx_shader_program;
	class bgfx_shader_program_factory;

	struct bgfx_prim2d_setup_params {
		MAKE_PROPERTY(bgfx_prim2d_setup_params, unsigned int, max_buffered_rect_count, 600);
		MAKE_PROPERTY(bgfx_prim2d_setup_params, unsigned int, max_buffered_tri_count, 600);
	};

	class bgfx_prim2d : public prim2d {
	private:
		static const int RECT_VERTEX_COUNT;
		static const int RECT_INDEX_COUNT;
		static const int TRI_VERTEX_COUNT;
		static const int TRI_INDEX_COUNT;

	private:
		bgfx_context& _context;
		bgfx_shader_program_factory& _factory;

		bgfx_prim2d_setup_params _setup_params;
		unsigned int _max_buffered_vertices;
		dynamic_vector<bgfx_prim2d_vertex> _buffered_vertices;
		unsigned int _max_buffered_indices;
		dynamic_vector<uint16_t> _buffered_indices;

		bgfx_shader_program* _default_shader_program;
		render_state_block* _default_rs_block;
		
		bgfx::VertexDecl _vertex_decl;
		bgfx::DynamicVertexBufferHandle _vertex_buffer_handle;
		bgfx::DynamicIndexBufferHandle _index_buffer_handle;

		bool _is_rendering;
		shader_program* _rendering_shader_program;

	public:
		bgfx_prim2d(bgfx_context& context, bgfx_shader_program_factory& factory);
		virtual ~bgfx_prim2d();

		void setup(const bgfx_prim2d_setup_params& params);
		void teardown();
		
		virtual void begin_rendering(const rect& viewport_area) override;
		virtual void begin_rendering(const rect& viewport_area, shader_program& shader_program, render_state_block* rs_block) override;
		virtual void end_rendering() override;
		virtual void set_shader_program(shader_program& shader_program) override;
		virtual void set_texture(texture& texture) override;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) override;
		virtual void render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) override;

	private:
		void flush_and_submit_buffer();
	};

}