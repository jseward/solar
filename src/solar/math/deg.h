#pragma once

#include "unit_convert.h"
#include "solar/utility/assert.h"

namespace solar {

	class deg {
	public:
		float _value;

	public:
		deg()
			: _value(0.f) {
		}

		deg(float value)
			: _value(value) {
		}

		float to_rad() const {
			return deg_to_rad(_value);
		}

		deg operator/(float d) const {
			ASSERT(d > 0.f);
			return deg(_value / d);
		}
	};

}