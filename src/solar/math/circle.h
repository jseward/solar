#pragma once

#include "vec2.h"

namespace solar {

	class circle {
	private:
		vec2 _center;
		float _radius;

	public:
		circle();
		circle(const vec2& center, float radius);

		bool is_position_within(const vec2& p) const;
	};

}