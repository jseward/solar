#pragma once

namespace solar {

	class vec2 {
	private:
		float _x;
		float _y;

	public:
		vec2();
		vec2(float x, float y);

		float get_x() const;
		float get_y() const;
	};

}