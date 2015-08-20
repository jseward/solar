#include "pointf.h"

namespace solar {

	pointf::pointf()
		: _x(0)
		, _y(0) {
	}

	pointf::pointf(float x, float y)
		: _x(x)
		, _y(y) {
	}

	float pointf::get_x() const {
		return _x;
	}

	void pointf::set_x(float x) {
		_x = x;
	}

	float pointf::get_y() const {
		return _y;
	}

	void pointf::set_y(float y) {
		_y = y;
	}

	const float* pointf::as_raw_float_array() const {
		return &_x;
	}

	std::ostream& operator<<(std::ostream& os, const pointf& p) {
		return os << "{ " << p._x << ", " << p._y << " }";
	}
}
