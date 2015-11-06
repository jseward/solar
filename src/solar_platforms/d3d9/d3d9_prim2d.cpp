#include "d3d9_prim2d.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/rendering/shaders/shader.h"
#include "solar/rendering/colors.h"
#include "d3d9_shader_factory.h"

namespace solar {

	const char* d3d9_prim2d::shader_param_names::TEXTURE = "diffuse_texture";

	const char* d3d9_prim2d::DEFAULT_SHADER_STRING =
		"texture diffuse_texture;\r\n"
		"sampler diffuse_sampler = sampler_state{\r\n"
		"	Texture = <diffuse_texture>;\r\n"
		"	MipFilter = LINEAR;\r\n"
		"	MinFilter = LINEAR;\r\n"
		"	MagFilter = LINEAR;\r\n"
		"	AddressU = CLAMP;\r\n"
		"	AddressV = CLAMP;\r\n"
		"	SRGBTexture = FALSE;\r\n"
		"};\r\n"
		"\r\n"
		"struct vs_input {\r\n"
		"	float4 position : POSITION;\r\n"
		"	float4 color : COLOR0;\r\n"
		"	float2 tex_coord : TEXCOORD0;\r\n"
		"};\r\n"
		"\r\n"
		"struct ps_input {\r\n"
		"	float4 position : POSITION;\r\n"
		"	float4 color : COLOR0;\r\n"
		"	float2 tex_coord : TEXCOORD0;\r\n"
		"};\r\n"
		"\r\n"
		"ps_input vs_render(vs_input input) {\r\n"
		"	ps_input output;\r\n"
		"	output.position = input.position;\r\n"
		"	output.color = input.color;\r\n"
		"	output.tex_coord = input.tex_coord;\r\n"
		"	return output;\r\n"
		"}\r\n"
		"\r\n"
		"float4 ps_render(ps_input input) : COLOR{\r\n"
		"	return tex2D(diffuse_sampler, input.tex_coord) * input.color;\r\n"
		"}\r\n"
		"\r\n"
		"technique render{ pass pass_0{\r\n"
		"	VertexShader = compile vs_2_0 vs_render();\r\n"
		"	PixelShader = compile ps_2_0 ps_render();\r\n"
		"	ZEnable = FALSE;\r\n"
		"	ZWriteEnable = FALSE;\r\n"
		"	AlphaBlendEnable = TRUE;\r\n"
		"	SrcBlend = SRCALPHA;\r\n"
		"	DestBlend = INVSRCALPHA;\r\n"
		"	AlphaTestEnable = FALSE;\r\n"
		"}}\r\n";

	d3d9_prim2d::d3d9_prim2d(d3d9_context& context, d3d9_shader_factory& shader_factory)
		: _context(context)
		, _shader_factory(shader_factory)
		, _default_shader(nullptr)
		, _white_texture(colors::WHITE)
		, _is_rendering(false)
		, _shader(nullptr) {
	}

	d3d9_prim2d::~d3d9_prim2d() {
		ASSERT(!_is_rendering);
		ASSERT(_buffered_rects.empty());
		ASSERT(_buffered_tris.empty());
		ASSERT(_default_shader == nullptr);
	}

	void d3d9_prim2d::setup(const d3d9_prim2d_setup_params& params) {
		_setup_params = params;

		_vertex_buffer.set_max_vertices(params.get_max_buffered_rect_count() * d3d9_prim2d_rect::VERTEX_COUNT);
		_index_buffer.set_max_indices(params.get_max_buffered_rect_count() * d3d9_prim2d_rect::INDEX_COUNT);
		_buffered_rects.reserve(params.get_max_buffered_rect_count());
		_buffered_tris.reserve(params.get_max_buffered_tri_count());

		ASSERT(_default_shader == nullptr);
		_default_shader = _shader_factory.create_embeded_code_shader(DEFAULT_SHADER_STRING);

		_context.add_device_event_handler(this);
	}

	void d3d9_prim2d::teardown() {
		_shader_factory.release_embeded_code_shader(_default_shader);
		_default_shader = nullptr;

		_context.remove_device_event_handler(this);
	}

	void d3d9_prim2d::begin_rendering(const rect& viewport_area) {
		ASSERT(_default_shader != nullptr);
		begin_rendering(viewport_area, *_default_shader);
	}

	void d3d9_prim2d::begin_rendering(const rect& viewport_area, shader& shader) {
		UNUSED_PARAMETER(viewport_area);
		ASSERT(!_is_rendering);
		ASSERT(_shader == nullptr);
		ASSERT(_texture == nullptr);
		_is_rendering = true;
		_shader = &shader;
		_shader->set_platform_texture(shader_param_names::TEXTURE, _white_texture.get());
		_shader->start("render");
	}

	void d3d9_prim2d::end_rendering() {
		flush_all();

		_shader->stop();
		_shader = nullptr;
		_texture = nullptr;

		ASSERT(_is_rendering);
		_is_rendering = false;
	}

	void d3d9_prim2d::set_shader(shader& shader) {
		ASSERT(_is_rendering);
		if (_shader != &shader) {
			flush_all();
			_shader->stop();
			_shader = &shader;
			if (_texture != nullptr) {
				_shader->set_texture(shader_param_names::TEXTURE, *_texture);
			}
			else {
				_shader->set_platform_texture(shader_param_names::TEXTURE, _white_texture.get());
			}
			_shader->start("render");
		}
	}

	void d3d9_prim2d::set_texture(texture& texture) {
		ASSERT(_is_rendering);
		if (_texture != &texture) {
			flush_all();
			_shader->set_texture(shader_param_names::TEXTURE, texture);
			_shader->commit_param_changes();
			_texture = &texture;
		}
	}

	void d3d9_prim2d::render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) {
		ASSERT(_is_rendering);

		if (_buffered_rects.size() >= _setup_params.get_max_buffered_rect_count()) {
			flush_rects();
		}

		_buffered_rects.emplace_back(top_left, top_right, bottom_right, bottom_left, color, uvs);
	}

	void d3d9_prim2d::render_triangle(const vec2& p0, const uv& uv0, const vec2& p1, const uv& uv1, const vec2& p2, const uv& uv2, const color& color) {
		ASSERT(_is_rendering);

		if (_buffered_tris.size() >= _setup_params.get_max_buffered_tri_count()) {
			flush_tris();
		}

		_buffered_tris.emplace_back(p0, uv0, p1, uv1, p2, uv2, color);
	}

	void d3d9_prim2d::flush_all() {
		flush_rects();
		flush_tris();
	}

	void d3d9_prim2d::flush_rects() {
		if (!_buffered_rects.empty()) {
			const int vertices_required = _buffered_rects.size() * d3d9_prim2d_rect::VERTEX_COUNT;
			const int indices_required = _buffered_rects.size() * d3d9_prim2d_rect::INDEX_COUNT;

			auto lr = lock_buffers_and_render_if_needed(vertices_required, indices_required);

			int vertex_offset = 0;
			int index_offset = 0;
			for (const auto& rect : _buffered_rects) {
				lr._vertices[0 + vertex_offset].set(rect._top_left, rect._color, rect._uvs.get_top_left());
				lr._vertices[1 + vertex_offset].set(rect._top_right, rect._color, rect._uvs.get_top_right());
				lr._vertices[2 + vertex_offset].set(rect._bottom_right, rect._color, rect._uvs.get_bottom_right());
				lr._vertices[3 + vertex_offset].set(rect._bottom_left, rect._color, rect._uvs.get_bottom_left());

				WORD index_value_offset = static_cast<WORD>(lr._vertices_begin + vertex_offset);
				lr._indices[0 + index_offset] = (0 + index_value_offset);
				lr._indices[1 + index_offset] = (1 + index_value_offset);
				lr._indices[2 + index_offset] = (2 + index_value_offset);
				lr._indices[3 + index_offset] = (2 + index_value_offset);
				lr._indices[4 + index_offset] = (3 + index_value_offset);
				lr._indices[5 + index_offset] = (0 + index_value_offset);

				vertex_offset += d3d9_prim2d_rect::VERTEX_COUNT;
				index_offset += d3d9_prim2d_rect::INDEX_COUNT;
			}

			unlock_buffers_and_render();

			_buffered_rects.clear();
		}
	}

	void d3d9_prim2d::flush_tris() {
		if (!_buffered_tris.empty()) {
			const int vertices_required = _buffered_tris.size() * d3d9_prim2d_tri::VERTEX_COUNT;
			const int indices_required = _buffered_tris.size() * d3d9_prim2d_tri::INDEX_COUNT;

			auto lr = lock_buffers_and_render_if_needed(vertices_required, indices_required);

			int vertex_offset = 0;
			int index_offset = 0;
			for (const auto& tri : _buffered_tris) {
				lr._vertices[0 + vertex_offset].set(tri._p0, tri._color, tri._uv0);
				lr._vertices[1 + vertex_offset].set(tri._p1, tri._color, tri._uv1);
				lr._vertices[2 + vertex_offset].set(tri._p2, tri._color, tri._uv2);

				WORD index_value_offset = static_cast<WORD>(lr._vertices_begin + vertex_offset);
				lr._indices[0 + index_offset] = (0 + index_value_offset);
				lr._indices[1 + index_offset] = (1 + index_value_offset);
				lr._indices[2 + index_offset] = (2 + index_value_offset);

				vertex_offset += d3d9_prim2d_tri::VERTEX_COUNT;
				index_offset += d3d9_prim2d_tri::INDEX_COUNT;
			}

			unlock_buffers_and_render();

			_buffered_tris.clear();
		}
	}

	d3d9_prim2d::lock_buffers_result d3d9_prim2d::lock_buffers_and_render_if_needed(int vertices_required, int indices_required) {
		if (
			vertices_required > _vertex_buffer.get_vertices_available() ||
			indices_required > _index_buffer.get_indices_available()) {

			_vertex_buffer.render_indexed_tris(_context.get_device(), _index_buffer);
			_vertex_buffer.discard_contents();
			_index_buffer.discard_contents();
		}

		int vertices_begin = _vertex_buffer.get_vertices_used();
		d3d9_prim2d_vertex* vertices = _vertex_buffer.lock(vertices_required);
		WORD* indices = _index_buffer.lock(indices_required);

		return lock_buffers_result(vertices, indices, vertices_begin);
	}

	void d3d9_prim2d::unlock_buffers_and_render() {
		_vertex_buffer.unlock();
		_index_buffer.unlock();

		_vertex_buffer.render_indexed_tris(_context.get_device(), _index_buffer);
		_vertex_buffer.move_past_contents();
		_index_buffer.move_past_contents();
	}

	void d3d9_prim2d::on_device_created(IDirect3DDevice9* device) {
		_white_texture.on_device_created(device);
		_vertex_buffer.on_device_created(device);
		_index_buffer.on_device_created(device);
	}

	void d3d9_prim2d::on_device_released(IDirect3DDevice9* device) {
		_white_texture.on_device_released(device);
		_vertex_buffer.on_device_released(device);
		_index_buffer.on_device_released(device);
	}

	void d3d9_prim2d::on_device_reset(IDirect3DDevice9* device) {
		_white_texture.on_device_reset(device);
		_vertex_buffer.on_device_reset(device);
		_index_buffer.on_device_reset(device);
	}

	void d3d9_prim2d::on_device_lost(IDirect3DDevice9* device) {
		_white_texture.on_device_lost(device);
		_vertex_buffer.on_device_lost(device);
		_index_buffer.on_device_lost(device);
	}
}