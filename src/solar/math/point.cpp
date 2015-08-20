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

	int point::get_x() const {
		return _x;
	}

	void point::set_x(int x) {
		_x = x;
	}

	int point::get_y() const {
		return _y;
	}

	void point::set_y(int y) {
		_y = y;
	}

	point& point::operator+=(const point& rhs) {
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const point& p) {
		return os << "{ " << p._x << ", " << p._y << " }";
	}
}
