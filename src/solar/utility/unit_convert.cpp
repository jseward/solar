#include "unit_convert.h"

namespace solar {

	float bytes_to_mb(unsigned int bytes) {
		return (float)bytes / (1024.f * 1024.f);
	}

	unsigned int mb_to_bytes(float mb) {
		return (unsigned int)(mb * 1024.f * 1024.f);
	}

}