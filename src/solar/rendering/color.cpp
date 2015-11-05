#include "color.h"

#include "solar/math/clamp.h"
#include "solar/math/math_helpers.h"
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

	std::array<float, 3> color::to_hsv() const {
		float c_max = std::max(std::max(_r, _g), _b);
		float c_min = std::min(std::min(_r, _g), _b);
		float c_delta = c_max - c_min;
			
		float h = 0.f;
		float s = 0.f;
		float v = 0.f;

		if (c_delta > 0.f) {
			if (c_max == _r) {
				h = 60.f * (std::fmodf(((_g - _b) / c_delta), 6.f));
			}
			else if (c_max == _g) {
				h = 60.f * (((_b - _r) / c_delta) + 2.f);
			}
			else if (c_max == _b) {
				h = 60.f * (((_r - _g) / c_delta) + 4.f);
			}
			else {
				ASSERT(false);
			}

			if (h < 0.f) {
				h += 360.f;
			}
				
			if (c_max > 0.f) {
				s = c_delta / c_max;
			}
		}

		v = c_max;

		return{ h, s, v };
	}

	color make_color_from_hsv(std::array<float, 3>& hsv) {
		return make_color_from_hsv(hsv[0], hsv[1], hsv[2]);
	}

	color make_color_from_hsv(float h, float s, float v) {
		ASSERT(is_within_inclusive(0.f, 360.f, h));
		ASSERT(is_within_inclusive(0.f, 1.f, s));
		ASSERT(is_within_inclusive(0.f, 1.f, v));

		float c = v * s;
		float h_prime = std::fmodf(h / 60.f, 6.f);
		float x = c * (1.f - std::fabs(std::fmodf(h_prime, 2.f) - 1.f));
		float m = v - c;

		float r = 0.f;
		float g = 0.f;
		float b = 0.f;

		if (0.f <= h_prime && h_prime < 1.f) {
			r = c;
			g = x;
			b = 0.f;
		}
		else if (1.f <= h_prime && h_prime < 2.f) {
			r = x;
			g = c;
			b = 0.f;
		}
		else if (2.f <= h_prime && h_prime < 3.f) {
			r = 0.f;
			g = c;
			b = x;
		}
		else if (3.f <= h_prime && h_prime < 4.f) {
			r = 0;
			g = x;
			b = c;
		}
		else if (4.f <= h_prime && h_prime < 5.f) {
			r = x;
			g = 0.f;
			b = c;
		}
		else if (5.f <= h_prime && h_prime < 6.f) {
			r = c;
			g = 0.f;
			b = x;
		}

		r += m;
		g += m;
		b += m;

		return color(r, g, b);
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

	color lerp(const color& c0, const color& c1, float t) {
		return color(
			lerp(c0._r, c1._r, t),
			lerp(c0._g, c1._g, t),
			lerp(c0._b, c1._b, t),
			lerp(c0._a, c1._a, t));
	}

}