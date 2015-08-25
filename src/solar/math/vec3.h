#pragma once

namespace solar {

	class vec3 {
	public:
		float _x;
		float _y;
		float _z;

	public:
		vec3();
		vec3(float x, float y, float z);

		bool operator==(const vec3& rhs) const;
		bool operator!=(const vec3& rhs) const;

		float get_length() const;
		float get_length_squared() const;

	public:
		friend vec3 operator/(const vec3& lhs, float rhs);
		friend vec3 operator-(const vec3& lhs, const vec3& rhs);

	public:
		friend vec3 normalize(const vec3& in);
		friend float dot(const vec3& u, const vec3& v);
		friend vec3 cross(const vec3& u, const vec3& v);
		friend bool are_collinear(const vec3& u, const vec3& v);
	};

}