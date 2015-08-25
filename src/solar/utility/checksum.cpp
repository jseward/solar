#include "checksum.h"

#include "solar/strings/string_helpers.h"
#include "solar/time/date_time.h"
#include "solar/utility/assert.h"

namespace solar {

	checksum::checksum()
		: _value(0) {
	}

	checksum::checksum(int int_value)
		: _value(int_value) {
	}

	bool checksum::operator==(const checksum& rhs) const {
		return (_value == rhs._value);
	}

	bool checksum::operator!=(const checksum& rhs) const {
		return (_value != rhs._value);
	}

	checksum& checksum::add_bool(bool v) {
		_value += v ? 1 : 0;
		return *this;
	}

	checksum& checksum::add_int(int v) {
		_value += v;
		return *this;
	}

	checksum& checksum::add_int64(__int64 v) {
		_value += calculate_fletcher_checksum_of_type<__int64>(v);
		return *this;
	}

	checksum& checksum::add_float(float v) {
		_value += calculate_fletcher_checksum_of_type<float>(v);
		return *this;
	}

	checksum& checksum::add_vec3(const vec3& v) {
		add_checksum_at_index(0, calculate_fletcher_checksum_of_type<float>(v._x));
		add_checksum_at_index(1, calculate_fletcher_checksum_of_type<float>(v._y));
		add_checksum_at_index(2, calculate_fletcher_checksum_of_type<float>(v._z));
		return *this;
	}

	checksum& checksum::add_string(const char* v) {
		_value += get_string_hash_code(v);
		return *this;
	}

	checksum& checksum::add_string(const std::string& v) {
		return add_string(v.c_str());
	}

	checksum& checksum::add_checksum(const checksum& checksum) {
		_value += checksum._value;
		return *this;
	}

	checksum& checksum::add_checksum_at_index(int index, const checksum& checksum_at_index) {
		_value += (checksum_at_index._value * (index + 1));
		_value += index;
		return *this;
	}

	checksum& checksum::add_date_time(const date_time& dt) {
		add_int64(dt.get_raw_system_time());
		return *this;
	}

	int checksum::to_int() const {
		return _value;
	}

	int checksum::calculate_fletcher_checksum(const unsigned int* data_32, unsigned int data_32_count) const {
		//http://en.wikipedia.org/wiki/Fletcher's_checksum

		unsigned int sum_a = 0xFFFF;
		unsigned int sum_b = 0xFFFF;

		const unsigned short* data_16 = reinterpret_cast<const unsigned short*>(data_32);
		unsigned int data_16_count = data_32_count * 2;
		while (data_16_count != 0) {

			unsigned int loop_count = (data_16_count > 360) ? 360 : data_16_count;
			data_16_count -= loop_count;

			for (unsigned int i = 0; i < loop_count; ++i) {
				sum_a += *data_16;
				sum_b += sum_a;
				data_16++;
			}

			sum_a = (sum_a & 0xFFFF) + (sum_a >> 16);
			sum_b = (sum_b & 0xFFFF) + (sum_b >> 16);
		}

		sum_a = (sum_a & 0xFFFF) + (sum_a >> 16);
		sum_b = (sum_b & 0xFFFF) + (sum_b >> 16);

		return static_cast<int>( (sum_b << 16) | sum_a );
	}

}
