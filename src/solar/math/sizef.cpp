#include "sizef.h"

namespace solar {

	sizef::sizef()
		: _width(0)
		, _height(0) {
	}

	sizef::sizef(float w, float h)
		: _width(w)
		, _height(h) {
	}

	std::ostream& operator<<(std::ostream& os, const sizef& s) {
		return os << "[" << s._width << ", " << s._height << "]";
	}

}
