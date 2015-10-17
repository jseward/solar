#pragma once

#include "solar/input/keyboard_key.h"

namespace solar {

	class window_key_event_params {
	public:
		const keyboard_key _key;

	public:
		window_key_event_params(keyboard_key key)
			: _key(key) {
		}
	};

}