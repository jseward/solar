#pragma once

#include "solar/utility/assert.h"
#include "solar/utility/unit_convert.h"

namespace solar {

	enum class archive_int_compression_type {
		NONE,
		RANGE,
		SOFT_MAX_COUNT
	};

	class archive_int_compression {
	public:
		archive_int_compression_type _type;
		int _min_value;
		int _max_value;
		int _common_value;

	public:
		archive_int_compression() 
			: _type(archive_int_compression_type::NONE)
			, _min_value(0)
			, _max_value(0)
			, _common_value(0) {
		}

		unsigned int get_bits_required() const {
			ASSERT(_type == archive_int_compression_type::RANGE);
			unsigned int range = _max_value - _min_value;
			unsigned int bits_required = bytes_to_bits(sizeof(unsigned int)) - get_leading_zero_count(range);
			ASSERT(bits_required <= 32);
			return bits_required;
		}

		unsigned int get_leading_zero_count(unsigned int value) const {
			//RakNet BitStream::NumberOfLeadingZeroes
			unsigned int x = value;
			unsigned int y = 0;
			int n = 32;
			y = x >> 16; if (y != 0) { n = n - 16; x = y; }
			y = x >> 8; if (y != 0) { n = n - 8; x = y; }
			y = x >> 4; if (y != 0) { n = n - 4; x = y; }
			y = x >> 2; if (y != 0) { n = n - 2; x = y; }
			y = x >> 1; if (y != 0) return n - 2;
			return (n - x);
		}
	};

	inline archive_int_compression make_archive_int_compression_range(int min_value, int max_value) {
		ASSERT(min_value <= max_value);
		archive_int_compression c;
		c._type = archive_int_compression_type::RANGE;
		c._min_value = min_value;
		c._max_value = max_value;
		return c;
	}

	inline archive_int_compression make_archive_int_compression_soft_max_count(int max_count) {
		ASSERT(max_count >= 0);
		archive_int_compression c;
		c._type = archive_int_compression_type::SOFT_MAX_COUNT;
		c._max_value = max_count;
		return c;

	}

}