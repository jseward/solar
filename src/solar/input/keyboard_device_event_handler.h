#pragma once

#include "keyboard_key.h"

namespace solar {

	class keyboard_device_event_handler {
	public:
		virtual void on_key_down(keyboard_key key) = 0;
		virtual void on_key_up(keyboard_key key) = 0;
		virtual void on_char_received(wchar_t char_code) = 0;
	};

}