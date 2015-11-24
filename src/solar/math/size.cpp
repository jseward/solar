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

	bool size::operator==(const size& rhs) const {
		return (_width == rhs._width && _height == rhs._height);
	}

	bool size::operator!=(const size& rhs) const {
		return !(*this == rhs);
	}

	size operator*(const size& lhs, float rhs) {
		return size(
			static_cast<int>(lhs._width * rhs),
			static_cast<int>(lhs._height * rhs));
	}

	size operator-(const size& lhs, const size& rhs) {
		return size(
			lhs._width - rhs._width,
			lhs._height - rhs._height);
	}

	std::ostream& operator<<(std::ostream& os, const size& s) {
		return os << "[" << s._width << ", " << s._height << "]";
	}

}
