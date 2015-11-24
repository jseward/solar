#include "unit_convert.h"

#include "math_constants.h"

namespace solar {

	float bytes_to_mb(unsigned int bytes) {
		return (float)bytes / (1024.f * 1024.f);
	}

	unsigned int mb_to_bytes(float mb) {
		return (unsigned int)(mb * 1024.f * 1024.f);
	}

	unsigned int bytes_to_bits(unsigned int bytes) {
		return (bytes * 8);
	}

	unsigned int bits_to_bytes(unsigned int bits) {
		//always rounds up
		return ((bits + 7) >> 3);
	}

	float deg_to_rad(float deg) {
		return (deg * (PI / 180.f));
	}

	float rad_to_deg(float rad) {
		return (rad * (180.f / PI));
	}

}
