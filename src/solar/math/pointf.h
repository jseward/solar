#pragma once

#include <ostream>

namespace solar {

	class pointf {
	private:
		float _x;
		float _y;

	public:
		pointf();
		pointf(float x, float y);

		float get_x() const;
		void set_x(float x);

		float get_y() const;
		void set_y(float y);

		const float* as_raw_float_array() const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const pointf& p);
	};

}