#include "d3d9_prim2d.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/rendering/shaders/shader.h"
#include "solar/rendering/colors.h"

namespace solar {

	const char* d3d9_prim2d::shader_param_names::TEXTURE = "diffuse_texture";

	d3d9_prim2d::d3d9_prim2d(d3d9_context& context)
		: _context(context)
		, _white_texture(colors::WHITE)
		, _is_rendering(false)
		, _shader(nullptr) {
	}

	d3d9_prim2d::~d3d9_prim2d() {
	}

	void d3d9_prim2d::setup(const d3d9_prim2d_setup_params& params) {
		_setup_params = params;

		_vertex_buffer.set_max_vertices(params.get_max_buffered_rect_count() * d3d9_prim2d_rect::VERTEX_COUNT);
		_index_buffer.set_max_indices(params.get_max_buffered_rect_count() * d3d9_prim2d_rect::INDEX_COUNT);
		_buffered_rects.reserve(params.get_max_buffered_rect_count());

		_context.add_device_event_handler(this);
	}

	void d3d9_prim2d::teardown() {
		_context.remove_device_event_handler(this);
	}

	void d3d9_prim2d::begin_rendering(const rect& viewport_area, shader& shader) {
		UNUSED_PARAMETER(viewport_area);
		ASSERT(!_is_rendering);
		ASSERT(_shader == nullptr);
		ASSERT(_texture == nullptr);
		_is_rendering = true;
		_shader = &shader;
		_shader->set_platform_texture(shader_param_names::TEXTURE, _white_texture.get());
	}

	void d3d9_prim2d::end_rendering() {
		flush();

		ASSERT(_is_rendering);
		_is_rendering = false;
		_shader = nullptr;
		_texture = nullptr;
	}

	void d3d9_prim2d::set_shader(shader& shader) {
		ASSERT(_is_rendering);
		if (_shader != &shader) {
			flush();
			_shader = &shader;
			if (_texture != nullptr) {
				_shader->set_texture(shader_param_names::TEXTURE, *_texture);
			}
			else {
				_shader->set_platform_texture(shader_param_names::TEXTURE, _white_texture.get());
			}
		}
	}

	void d3d9_prim2d::set_texture(texture& texture) {
		ASSERT(_is_rendering);
		if (_texture != &texture) {
			flush();
			_shader->set_texture(shader_param_names::TEXTURE, texture);
			_texture = &texture;
		}
	}

	void d3d9_prim2d::render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) {
		if (_buffered_rects.size() >= _setup_params.get_max_buffered_rect_count()) {
			flush();
		}

		_buffered_rects.push_back(d3d9_prim2d_rect(top_left, top_right, bottom_right, bottom_left, color, uvs));
	}

	void d3d9_prim2d::flush() {
		if (!_buffered_rects.empty()) {
			_shader->start("render");
			flush_buffered_rects();
			_shader->stop();
		}
		else {
			_shader->forget_param_changes();
		}
	}

	void d3d9_prim2d::flush_buffered_rects() {
		int vertices_required = _buffered_rects.size() * d3d9_prim2d_rect::VERTEX_COUNT;
		int indices_required = _buffered_rects.size() * d3d9_prim2d_rect::INDEX_COUNT;

		if (
			vertices_required > _vertex_buffer.get_vertices_available() ||
			indices_required > _index_buffer.get_indices_available()) {

			_vertex_buffer.render_indexed_tris(_context.get_device(), _index_buffer);
			_vertex_buffer.discard_contents();
			_index_buffer.discard_contents();
		}

		int vertices_begin = _vertex_buffer.get_vertices_used();
		int vertex_offset = 0;
		int index_offset = 0;
		d3d9_prim2d_vertex* vertices = _vertex_buffer.lock(vertices_required);
		WORD* indices = _index_buffer.lock(indices_required);
		for (const auto& rect : _buffered_rects) {

			vertices[0 + vertex_offset].set(rect._top_left, rect._color, rect._uvs.get_top_left());
			vertices[1 + vertex_offset].set(rect._top_right, rect._color, rect._uvs.get_top_right());
			vertices[2 + vertex_offset].set(rect._bottom_right, rect._color, rect._uvs.get_bottom_right());
			vertices[3 + vertex_offset].set(rect._bottom_left, rect._color, rect._uvs.get_bottom_left());

			WORD index_value_offset = (WORD)(vertices_begin + vertex_offset);
			indices[0 + index_offset] = (0 + index_value_offset);
			indices[1 + index_offset] = (1 + index_value_offset);
			indices[2 + index_offset] = (2 + index_value_offset);
			indices[3 + index_offset] = (2 + index_value_offset);
			indices[4 + index_offset] = (3 + index_value_offset);
			indices[5 + index_offset] = (0 + index_value_offset);

			vertex_offset += d3d9_prim2d_rect::VERTEX_COUNT;
			index_offset += d3d9_prim2d_rect::INDEX_COUNT;
		}
		_vertex_buffer.unlock();
		_index_buffer.unlock();

		_vertex_buffer.render_indexed_tris(_context.get_device(), _index_buffer);
		_vertex_buffer.move_past_contents();
		_index_buffer.move_past_contents();

		_buffered_rects.clear();
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