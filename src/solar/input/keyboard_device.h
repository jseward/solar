#pragma once

#include "keyboard_device_event_handler.h"

namespace solar {

	class keyboard_device {
	public:
		virtual void add_event_handler(keyboard_device_event_handler* handler) = 0;
		virtual void remove_event_handler(keyboard_device_event_handler* handler) = 0;
		virtual bool is_alt_down() const = 0;
		virtual bool is_ctrl_down() const = 0;
		virtual bool is_shift_down() const = 0;
	};

}