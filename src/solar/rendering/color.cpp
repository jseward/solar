#include "color.h"

#include "solar/math/clamp.h"
#include "solar/strings/string_convert.h"
#include "solar/utility/verify.h"

namespace solar {

	color::color() 
		: _r(0.f)
		, _g(0.f)
		, _b(0.f)
		, _a(0.f) {
	}

	color::color(float r, float g, float b)
		: _r(r)
		, _g(g)
		, _b(b)
		, _a(1.f) {
	}

	color::color(float r, float g, float b, float a)
		: _r(r)
		, _g(g)
		, _b(b)
		, _a(a) {
	}

	color::color(const color& copy, float a)
		: _r(copy._r)
		, _g(copy._g)
		, _b(copy._b)
		, _a(a) {
	}

	float color::get_r() const {
		return _r;
	}

	float color::get_g() const {
		return _g;
	}

	float color::get_b() const {
		return _g;
	}

	float color::get_a() const {
		return _a;
	}

	uint32_t color::to_argb32() const {
		return 
			(convert_to_byte(_a) << 24) | 
			(convert_to_byte(_r) << 16) |
			(convert_to_byte(_g) << 8) |
			(convert_to_byte(_b) << 0);
	};

	uint8_t color::convert_to_byte(float v) const {
		return static_cast<uint8_t>(clamp(0.f, 1.f, v) * 255.0f + 0.5f);
	}

	std::string color::to_string() const {
		std::string s;
		s += hex_value_to_string(convert_to_byte(_a));
		s += hex_value_to_string(convert_to_byte(_r));
		s += hex_value_to_string(convert_to_byte(_g));
		s += hex_value_to_string(convert_to_byte(_b));
		return s;		
	}

	bool try_make_color_from_string(color& c, const char* s) {
		unsigned int argb = 0;
		if (!try_parse_hex_value_from_string(argb, s)) {
			return false;
		}
		c = make_color_from_argb(argb);
		return true;
	}

	color make_color_from_string(const char* s) {
		color c;
		VERIFY(try_make_color_from_string(c, s));
		return c;
	}

	color make_color_from_argb(uint32_t argb) {
		const float f = 1.0f / 255.0f;
		unsigned char a = static_cast<unsigned char>(argb >> 24);
		unsigned char r = static_cast<unsigned char>(argb >> 16);
		unsigned char g = static_cast<unsigned char>(argb >> 8);
		unsigned char b = static_cast<unsigned char>(argb >> 0);
		return color(
			f * static_cast<float>(r),
			f * static_cast<float>(g),
			f * static_cast<float>(b),
			f * static_cast<float>(a));
	}

}