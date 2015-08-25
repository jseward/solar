#pragma once

#include <ostream>

namespace solar {

	class point {
	public:
		int _x;
		int _y;

	public:
		point();
		point(int x, int y);

		bool operator==(const point& rhs) const;
		bool operator!=(const point& rhs) const;

		point& operator+=(const point& rhs);

	public:
		friend std::ostream& operator<<(std::ostream& os, const point& p);
		friend point operator+(const point& lhs, const point& rhs);
		friend point operator-(const point& lhs, const point& rhs);
	};

}