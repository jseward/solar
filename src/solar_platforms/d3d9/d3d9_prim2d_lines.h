#pragma once

#include "solar/rendering/primatives/prim2d_lines.h"
#include "d3d9_context.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_prim2d_lines
		: public prim2d_lines
		, public d3d9_device_event_handler {

	private:
		d3d9_context& _context;
		ID3DXLine* _d3dx_line;

		bool _is_rendering;
		D3DVIEWPORT9 _viewport;

	public:
		d3d9_prim2d_lines(d3d9_context& context);
		virtual ~d3d9_prim2d_lines();
		
		void setup();
		void teardown();

		virtual bool is_rendering() const override;
		virtual void begin_rendering(float line_width) override;
		virtual void end_rendering() override;
		virtual void render_segments(const vec2* points, unsigned int point_count, const color& color) override;

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}