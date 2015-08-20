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

	float sizef::get_width() const {
		return _width;
	}

	void sizef::set_width(float w) {
		_width = w;
	}

	float sizef::get_height() const {
		return _height;
	}

	void sizef::set_height(float h) {
		_height = h;
	}

	std::ostream& operator<<(std::ostream& os, const sizef& s) {
		return os << "[" << s._width << ", " << s._height << "]";
	}

}
