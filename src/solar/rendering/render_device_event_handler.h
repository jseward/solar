#pragma once

#include "solar/math/size.h"

namespace solar {

	class render_device_event_handler {
	public:
		virtual void on_render_device_backbuffer_size_changed(const size& new_size) = 0;
	};

}