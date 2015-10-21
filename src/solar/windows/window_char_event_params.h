#pragma once

#include "solar/input/keyboard_device.h"

namespace solar {

	class window_char_event_params {
	public:
		wchar_t _char;
		const keyboard_device& _keyboard_device;

	public:
		window_char_event_params(wchar_t c, const keyboard_device& keyboard_device)
			: _char(c)
			, _keyboard_device(keyboard_device) {
		}
	};

}
