#include "viewport.h"

#include "solar/utility/type_convert.h"

namespace solar {

	viewport::viewport()
		: _x(0)
		, _y(0)
		, _width(0)
		, _height(0)
		, _min_z(0.f)
		, _max_z(1.f) {
	}

	float viewport::get_aspect_ratio() const {
		if (_height > 0) {
			return uint_to_float(_width) / uint_to_float(_height);
		}
		return 0.f;
	}

	viewport& viewport::set_area(const rect& area) {
		_x = area.get_left();
		_y = area.get_top();
		_width = area.get_width();
		_height = area.get_height();
		return *this;
	}

	viewport& viewport::set_min_max_z(float min_z, float max_z) {
		_min_z = min_z;
		_max_z = max_z;
		return *this;
	}

}