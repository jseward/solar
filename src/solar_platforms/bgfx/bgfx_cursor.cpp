#include "bgfx_cursor.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_cursor::bgfx_cursor()
		: _raw_location() {
	}

	bgfx_cursor::~bgfx_cursor() {
	}

	void bgfx_cursor::set_icon(cursor_icon& icon) {
		UNUSED_PARAMETER(icon);
		//todo
	}

	void bgfx_cursor::set_raw_location(const point& location) {
		_raw_location = location;
	}

	point bgfx_cursor::get_hotspot_location() const {
		point hotspot_location = _raw_location;

		//todo
		//if (_active_icon != nullptr) {
		//	hotspot_location += _active_icon->get_hotspot_offset();
		//}

		return hotspot_location;
	}

}