#pragma once

#include <vector>
#include "solar/rendering/render_device.h"
#include "opengl_context.h"

namespace solar {

	class opengl_render_device : public render_device 
	{
	private:
		opengl_context& _context;
		std::vector<render_device_event_handler*> _event_handlers;

	public:
		opengl_render_device(opengl_context& context);
		virtual ~opengl_render_device();

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
	};

}