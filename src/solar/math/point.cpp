#include "point.h"

namespace solar {

	point::point()
		: _x(0)
		, _y(0) {
	}

	point::point(int x, int y)
		: _x(x)
		, _y(y) {
	}

	bool point::operator==(const point& rhs) const {
		return _x == rhs._x && _y == rhs._y;
	}

	bool point::operator!=(const point& rhs) const {
		return !(*this == rhs);
	}

	point& point::operator+=(const point& rhs) {
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const point& p) {
		return os << "{ " << p._x << ", " << p._y << " }";
	}

	point operator+(const point& lhs, const point& rhs) {
		return point(
			lhs._x + rhs._x,
			lhs._y + rhs._y);
	}

	point operator-(const point& lhs, const point& rhs) {
		return point(
			lhs._x - rhs._x,
			lhs._y - rhs._y);
	}

}
