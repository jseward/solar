#pragma once

#include "solar/math/point.h"
#include "cursor_icon.h"

namespace solar {

	class cursor {
	public:
		virtual void set_icon(cursor_icon& icon) = 0;
		virtual void set_raw_location(const point& location) = 0;
		virtual point get_hotspot_location() const = 0;
	};

}