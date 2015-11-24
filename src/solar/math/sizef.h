#pragma once

#include <ostream>

namespace solar {

	class sizef {
	public:
		float _width;
		float _height;

	public:
		sizef();
		sizef(float w, float h);

	public:
		friend std::ostream& operator<<(std::ostream& os, const sizef& s);
	};

}