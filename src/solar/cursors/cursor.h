#pragma once

#include "solar/math/point.h"

namespace solar {

	//intent of this class is to abstract the platform specific visual display of a cursor - todo

	class cursor {
	private:
		point _raw_location;

	public:
		cursor();
		virtual ~cursor();

		point get_hotspot_location() const;
		void set_raw_location(const point& raw_location);
	};

}