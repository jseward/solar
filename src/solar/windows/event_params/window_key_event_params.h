#pragma once

#include "solar/input/keyboard_key.h"
#include "solar/input/keyboard_device.h"

namespace solar {
	
	class clipboard;

	class window_key_event_params {
	public:
		const keyboard_key _key;
		keyboard_device& _keyboard_device;
		clipboard& _clipboard;

	public:
		window_key_event_params(keyboard_key key, keyboard_device& keyboard_device, clipboard& clipboard)
			: _key(key) 
			, _keyboard_device(keyboard_device) 
			, _clipboard(clipboard) {
		}

		bool is_alt_down() const {
			return _keyboard_device.is_alt_down();
		}

		bool is_shift_down() const {
			return _keyboard_device.is_shift_down();
		}

		bool is_ctrl_down() const {
			return _keyboard_device.is_ctrl_down();
		}
	};

}