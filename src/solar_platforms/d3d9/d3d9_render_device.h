#pragma once

#include "solar/rendering/render_device.h"
#include "d3d9_context.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_render_device 
		: public render_device 
		, public d3d9_device_event_handler
	{

	private:
		d3d9_context& _context;
		std::vector<render_device_event_handler*> _event_handlers;

	public:
		d3d9_render_device(d3d9_context& context);
		virtual ~d3d9_render_device();

		void setup();
		void teardown();

		virtual void add_event_handler(render_device_event_handler* handler) override;
		virtual void remove_event_handler(render_device_event_handler* handler) override;
		virtual void attempt_render_scene(render_scene_functor& func) override;
		virtual bool begin_scene() override;
		virtual void end_scene() override;
		virtual void clear(const color& color) override;
		virtual void toggle_virtual_fullscreen();
		virtual viewport set_viewport(const viewport& new_viewport) override;
		virtual size get_backbuffer_size() const override;
		virtual render_state_block* create_render_state_block(const render_state_block_def& def) override;
		virtual void release_render_state_block(render_state_block* block) override;

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}