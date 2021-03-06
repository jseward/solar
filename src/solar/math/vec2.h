#pragma once

#include <ostream>

namespace solar {

	class vec2 {
	public:
		float _x;
		float _y;

	public:
		vec2();
		vec2(float x, float y);

		bool is_normalized() const;

		float get_length() const;
		float get_length_squared() const;

		vec2& operator+=(const vec2& rhs);
		vec2& operator-=(const vec2& rhs);

	public:
		friend vec2 operator+(const vec2& lhs, const vec2& rhs);
		friend vec2 operator-(const vec2& lhs, const vec2& rhs);
		friend vec2 operator*(const vec2& lhs, float k);
		friend vec2 operator/(const vec2& lhs, float k);
		friend vec2 operator-(const vec2& v);

	public:
		friend vec2 make_perpendicular(const vec2& in, float dir); //dir must be 1.f or -1.f
		friend vec2 normalize(const vec2& in);
		friend float get_distance(const vec2& v0, const vec2& v1);
		friend float get_distance_squared(const vec2& v0, const vec2& v1);
		friend float dot(const vec2& u, const vec2& v);
		friend float cross(const vec2& u, const vec2& v);
		friend float get_angle_between_using_any_dir(const vec2& u, const vec2& v);
		friend float get_angle_between_using_cc_only(const vec2& u, const vec2& v);
		friend bool is_approx(const vec2& v0, const vec2& v1, float tolerance);
		friend std::ostream& operator<<(std::ostream& os, const vec2& v);
	};

}