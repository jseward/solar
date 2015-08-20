#pragma once

#include <ostream>

namespace solar {

	class point {
	private:
		int _x;
		int _y;

	public:
		point();
		point(int x, int y);

		int get_x() const;
		void set_x(int x);

		int get_y() const;
		void set_y(int y);

		point& operator+=(const point& rhs);

	public:
		friend std::ostream& operator<<(std::ostream& os, const point& p);
	};

}