#pragma once

namespace solar {

	class vec4 {
	public:
		float _x;
		float _y;
		float _z;
		float _w;

	public:
		vec4();
		vec4(float x, float y, float z, float w);
	};

}