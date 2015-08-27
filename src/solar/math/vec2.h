#pragma once

namespace solar {

	class vec2 {
	public:
		float _x;
		float _y;

	public:
		vec2();
		vec2(float x, float y);

	public:
		friend vec2 operator+(const vec2& lhs, const vec2& rhs);
		friend vec2 operator*(const vec2& lhs, float rhs);
	};

}