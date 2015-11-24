#include "cursor.h"

namespace solar {

	cursor::cursor() {
	}

	cursor::~cursor() {
	}

	point cursor::get_hotspot_location() const {
		return _raw_location;
	}

	void cursor::set_raw_location(const point& raw_location) {
		_raw_location = raw_location;
	}

}
