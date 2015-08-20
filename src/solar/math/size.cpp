#include "size.h"
#include <array>
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	size::size()
		: _width(0)
		, _height(0) {
	}

	size::size(int w, int h)
		: _width(w)
		, _height(h) {
	}

	int size::get_width() const {
		return _width;
	}

	void size::set_width(int w) {
		_width = w;
	}

	int size::get_height() const {
		return _height;
	}

	void size::set_height(int h) {
		_height = h;
	}

	bool size::operator==(const size& rhs) const {
		return (_width == rhs._width && _height == rhs._height);
	}

	bool size::operator!=(const size& rhs) const {
		return !(*this == rhs);
	}

	size size::operator*(float rhs) const {
		return size(
			static_cast<int>(_width * rhs),
			static_cast<int>(_height * rhs));
	}

	std::ostream& operator<<(std::ostream& os, const size& s) {
		return os << "[" << s._width << ", " << s._height << "]";
	}

}
