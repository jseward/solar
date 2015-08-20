#pragma once

#include <stdint.h>
#include <string>

namespace solar {

	class color {
		
	private:
		float _r;
		float _g;
		float _b;
		float _a;

	public:
		color();
		color(float r, float g, float b);
		color(float r, float g, float b, float a);
		color(const color& copy, float a);

		float get_r() const;
		float get_g() const;
		float get_b() const;
		float get_a() const;

		uint32_t to_argb32() const;
		std::string to_string() const;

	private:
		uint8_t convert_to_byte(float v) const;

	public:
		friend bool try_make_color_from_string(color& c, const char* s);
		friend color make_color_from_string(const char* s);
		friend color make_color_from_argb(unsigned int argb);
	};

}