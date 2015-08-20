#pragma once

#include "solar/math/rect.h"

namespace solar {

	class viewport {
	public:
		unsigned int _x;
		unsigned int _y;
		unsigned int _width;
		unsigned int _height;
		float _min_z;
		float _max_z;

	public:
		viewport();

		float get_aspect_ratio() const;

		viewport& set_area(const rect& area);
		viewport& set_min_max_z(float min_z, float max_z);
	};


}