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

	const float* pointf::as_raw_float_array() const {
		return &_x;
	}

	std::ostream& operator<<(std::ostream& os, const pointf& p) {
		return os << "{ " << p._x << ", " << p._y << " }";
	}
}
