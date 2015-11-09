#pragma once

#include <vector>
#include "solar/rendering/render_device.h"
#include "bgfx_context.h"

namespace solar {

	class bgfx_render_device : public render_device 
	{
	private:
		bgfx_context& _context;
		std::vector<render_device_event_handler*> _event_handlers;

	public:
		bgfx_render_device(bgfx_context& context);
		virtual ~bgfx_render_device();

		virtual void add_event_handler(render_device_event_handler* handler) override;
		virtual void remove_event_handler(render_device_event_handler* handler) override;
		virtual void attempt_render_scene(render_scene_functor& func) override;
		virtual bool begin_scene() override;
		virtual void end_scene() override;
		virtual void clear(const color& color) override;
		virtual void toggle_virtual_fullscreen();
		virtual viewport set_viewport(const viewport& new_viewport) override;
		virtual size get_backbuffer_size() const override;
	};

}