#pragma once

#include "color.h"
#include "viewport.h"
#include "render_scene_functor.h"
#include "render_device_event_handler.h"

namespace solar {

	class render_device {
	public:
		virtual void add_event_handler(render_device_event_handler* handler) = 0;
		virtual void remove_event_handler(render_device_event_handler* handler) = 0;
		virtual void attempt_render_scene(render_scene_functor& func) = 0;
		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;
		virtual void clear(const color& color) = 0;
		virtual void toggle_virtual_fullscreen() = 0;
		virtual viewport set_viewport(const viewport& new_viewport) = 0;
		virtual size get_backbuffer_size() const = 0;
	};

}