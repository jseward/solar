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
#include "d3d9_prim2d_vertex.h"

namespace solar {

	struct d3d9_prim2d_setup_params {
		MAKE_PROPERTY(d3d9_prim2d_setup_params, unsigned int, max_buffered_rect_count, 600);
	};

	class d3d9_prim2d 
		: public prim2d
		, public d3d9_device_event_handler {

	private:
		struct shader_param_names {
			static const char* TEXTURE;
		};

	private:
		d3d9_context& _context;
		d3d9_prim2d_setup_params _setup_params;
		std::vector<d3d9_prim2d_rect> _buffered_rects;
		d3d9_solid_color_texture _white_texture;
		bool _is_rendering;
		shader* _shader;
		texture* _texture;
		d3d9_dynamic_vertex_buffer<d3d9_prim2d_vertex> _vertex_buffer;
		d3d9_dynamic_index_buffer _index_buffer;

	public:
		d3d9_prim2d(d3d9_context& context);
		virtual ~d3d9_prim2d();

		void setup(const d3d9_prim2d_setup_params& params);
		void teardown();
		
		virtual void begin_rendering(const rect& viewport_area, shader& shader) override;
		virtual void end_rendering() override;
		virtual void set_shader(shader& shader) override;
		virtual void set_texture(texture& texture) override;
		virtual void render_rect(const vec2& top_left, const vec2& top_right, const vec2& bottom_right, const vec2& bottom_left, const color& color, const simple_rect_uvs& uvs) override;

	private:
		void flush();
		void flush_buffered_rects();

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}