#include "vec4.h"

namespace solar {

	vec4::vec4()
		: _x(0.f)
		, _y(0.f) 
		, _z(0.f)
		, _w(0.f) {
	}

	vec4::vec4(float x, float y, float z, float w)
		: _x(x)
		, _y(y) 
		, _z(z)
		, _w(w) {
	}

}