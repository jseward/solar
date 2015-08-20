#pragma once

#include "mouse_device_event_handler.h"

namespace solar {

	class mouse_device {
	public:
		virtual void add_event_handler(mouse_device_event_handler* handler) = 0;
		virtual void remove_event_handler(mouse_device_event_handler* handler) = 0;
		virtual bool is_mouse_button_down(mouse_button button) const = 0;
	};

}