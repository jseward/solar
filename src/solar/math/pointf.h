#pragma once

#include <ostream>

namespace solar {

	class pointf {
	public:
		float _x;
		float _y;

	public:
		pointf();
		pointf(float x, float y);

		const float* as_raw_float_array() const;

	public:
		friend std::ostream& operator<<(std::ostream& os, const pointf& p);
	};

}