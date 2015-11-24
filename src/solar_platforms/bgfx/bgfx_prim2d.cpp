#include "bgfx_prim2d.h"

#include "solar/utility/unused_parameter.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/containers/container_helpers.h"
#include "bgfx_context.h"
#include "bgfx_shader_program_factory.h"
#include "bgfx_prim2d_vertex.h"
#include "./shaders/prim2d_shaders_embedded_code.h"

namespace solar {

	const int bgfx_prim2d::RECT_VERTEX_COUNT = 4;
	const int bgfx_prim2d::RECT_INDEX_COUNT = 6;
	const int bgfx_prim2d::TRI_VERTEX_COUNT = 3;
	const int bgfx_prim2d::TRI_INDEX_COUNT = 3;

	bgfx_prim2d::bgfx_prim2d(bgfx_context& context, bgfx_shader_program_factory& factory)
		: _context(context)
		, _factory(factory)
		
		, _setup_params()
		, _max_buffered_vertices(0)
		, _buffered_vertices()
		, _max_buffered_indices(0)
		, _buffered_indices()

		, _default_shader_program(nullptr)
		, _default_rs_block(nullptr) 

		, _vertex_decl()
		, _vertex_buffer_handle(BGFX_INVALID_HANDLE)
		, _index_buffer_handle(BGFX_INVALID_HANDLE)

		, _is_rendering(false)
		, _rendering_shader_program(nullptr) {
	}

	bgfx_prim2d::~bgfx_prim2d() {
		ASSERT(_default_shader_program == nullptr);
		ASSERT(_default_rs_block == nullptr);
		ASSERT(!bgfx::isValid(_vertex_buffer_handle));
		ASSERT(!bgfx::isValid(_index_buffer_handle));
		ASSERT(!_is_rendering);
		ASSERT(_rendering_shader_program == nullptr);
	}

	void bgfx_prim2d::setup(const bgfx_prim2d_setup_params& params) {
		_setup_params = params;

		auto renderer_type = bgfx::getRendererType();

		bgfx_prim2d_vertex::build_decl(_vertex_decl, renderer_type);
		ASSERT(_vertex_decl.getStride() == sizeof(bgfx_prim2d_vertex));

		_max_buffered_vertices = std::max(
			params.get_max_buffered_rect_count() * RECT_VERTEX_COUNT,
			params.get_max_buffered_tri_count() * TRI_VERTEX_COUNT);
		_vertex_buffer_handle = bgfx::createDynamicVertexBuffer(_max_buffered_vertices, _vertex_decl, BGFX_BUFFER_NONE);
		_buffered_vertices.reserve(_max_buffered_vertices);

		_max_buffered_indices = std::max(
			params.get_max_buffered_rect_count() * RECT_INDEX_COUNT,
			params.get_max_buffered_tri_count() * TRI_INDEX_COUNT);
		_index_buffer_handle = bgfx::createDynamicIndexBuffer(_max_buffered_indices, BGFX_BUFFER_NONE); //16-bit indices due to lack of BGFX_BUFFER_INDEX32
		_buffered_indices.reserve(_max_buffered_indices);
		
		_default_shader_program = _factory.create_embeded_code_shader_program(
			get_prim2d_vs_embedded_code(renderer_type),
			get_prim2d_fs_embedded_code(renderer_type));

		_default_rs_block = _context.create_render_state_block(render_state_block_def()
			.set_depth_write(render_state_depth_write::DISABLED)
			.set_depth_compare_func(render_state_compare_func::NONE)
			.set_blend(render_state_blend_type::SRC_ALPHA, render_state_blend_type::INV_SRC_ALPHA));
	}

	void bgfx_prim2d::teardown() {
		_factory.release_embeded_code_shader_program(_default_shader_program);
		_default_shader_program = nullptr;

		_context.release_render_state_block(_default_rs_block);
		_default_rs_block = nullptr;

		bgfx::destroyDynamicVertexBuffer(_vertex_buffer_handle);
		_vertex_buffer_handle = BGFX_INVALID_HANDLE;

		bgfx::destroyDynamicIndexBuffer(_index_buffer_handle);
		_index_buffer_handle = BGFX_INVALID_HANDLE;
	}

	void bgfx_prim2d::begin_rendering(const rect& viewport_area) {
		ASSERT(_default_shader_program != nullptr);
		begin_rendering(viewport_area, *_default_shader_program, _default_rs_block);
	}

	void bgfx_prim2d::begin_rendering(const rect& viewport_area, shader_program& shader_program, render_state_block* rs_block) {
		UNUSED_PARAMETER(viewport_area);

		ASSERT(!_is_rendering);
		_is_rendering = true;

		ASSERT(_rendering_shader_program == nullptr);
		_rendering_shader_program = &shader_program;
		//TODO - solid color texture!
		ASSERT(false);
		_rendering_shader_program->start();

		ASSERT(rs_block != nullptr);
		rs_block->commit();
	}

	void bgfx_prim2d::end_rendering() {
		flush_and_submit_buffer();

		IF_VERIFY(_rendering_shader_program != nullptr) {
			_rendering_shader_program->stop();
			_rendering_shader_program = nullptr;
		}

		ASSERT(_is_rendering);
		_is_rendering = false;
	}

	void bgfx_prim2d::set_shader_program(shader_program& shader_program) {
		//todo
		UNUSED_PARAMETER(shader_program);
		ASSERT(false);
	}

	void bgfx_prim2d::set_texture(texture& texture) {
		//todo
		UNUSED_PARAMETER(texture);
		ASSERT(false);
	}

	void bgfx_prim2d::render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) {
		ASSERT(_is_rendering);

		if (
			((_buffered_vertices.size() + RECT_VERTEX_COUNT) > _max_buffered_vertices) ||
			((_buffered_indices.size() + RECT_INDEX_COUNT) > _max_buffered_indices)) {
			
			flush_and_submit_buffer();
		}

		uint16_t index_begin = uint32_to_uint16(_buffered_vertices.size());

		_buffered_vertices.emplace_back(top_left, color, uvs.get_top_left());
		_buffered_vertices.emplace_back(top_right, color, uvs.get_top_right());
		_buffered_vertices.emplace_back(bottom_right, color, uvs.get_bottom_right());
		_buffered_vertices.emplace_back(bottom_left, color, uvs.get_bottom_left());

		_buffered_indices.push_back(index_begin + 0);
		_buffered_indices.push_back(index_begin + 1);
		_buffered_indices.push_back(index_begin + 2);
		_buffered_indices.push_back(index_begin + 2);
		_buffered_indices.push_back(index_begin + 3);
		_buffered_indices.push_back(index_begin + 0);
	}

	void bgfx_prim2d::render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) {
		if (
			((_buffered_vertices.size() + TRI_VERTEX_COUNT) > _max_buffered_vertices) ||
			((_buffered_indices.size() + TRI_INDEX_COUNT) > _max_buffered_indices)) {

			flush_and_submit_buffer();
		}

		uint16_t index_begin = uint32_to_uint16(_buffered_vertices.size());

		_buffered_vertices.emplace_back(p0, color, uv0);
		_buffered_vertices.emplace_back(p1, color, uv1);
		_buffered_vertices.emplace_back(p2, color, uv2);

		_buffered_indices.push_back(index_begin + 0);
		_buffered_indices.push_back(index_begin + 1);
		_buffered_indices.push_back(index_begin + 2);
	}

	void bgfx_prim2d::flush_and_submit_buffer() {
		if (_buffered_vertices.size() > 0) {
			bgfx::updateDynamicVertexBuffer(_vertex_buffer_handle, 0, bgfx::copy(_buffered_vertices.data(), get_data_size(_buffered_vertices)));
			bgfx::setVertexBuffer(_vertex_buffer_handle, _buffered_vertices.size());

			bgfx::updateDynamicIndexBuffer(_index_buffer_handle, 0, bgfx::copy(_buffered_indices.data(), get_data_size(_buffered_indices)));
			bgfx::setIndexBuffer(_index_buffer_handle, _buffered_indices.size());

			ASSERT(_rendering_shader_program != nullptr);
			auto program_handle = static_cast<bgfx_shader_program*>(_rendering_shader_program)->get_program_handle();
			bgfx::submit(_context.get_current_view_id(), program_handle, 0);

			_buffered_vertices.clear_fast_no_destruct();
			_buffered_indices.clear_fast_no_destruct();
		}
	}

}