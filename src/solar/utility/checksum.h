#pragma once

#include <string>
#include "solar/math/vec3.h"
#include "solar/math/size.h"
#include "solar/utility/assert.h"

namespace solar {

	class date_time;

	class checksum {
	private:
		unsigned int _value;

	public:
		checksum();
		checksum(int int_value);
		
		bool operator==(const checksum& rhs) const;
		bool operator!=(const checksum& rhs) const;

		checksum& add_bool(bool v);
		checksum& add_int(int v);
		checksum& add_int64(__int64 v);
		checksum& add_string(const char* v);
		checksum& add_string(const std::string& v);
		checksum& add_float(float v);
		checksum& add_vec3(const vec3& v);
		checksum& add_size(const size& v);
		checksum& add_checksum(const checksum& checksum);
		checksum& add_checksum_at_index(int index, const checksum& checksum_at_index);
		template<typename Enum> checksum& add_enum(Enum v);
		checksum& add_date_time(const date_time& dt);
		template<typename Con, typename Pr> checksum& add_container(const Con& in, Pr getChecksum);

		int to_int() const;

	private:
		template<typename T> int calculate_fletcher_checksum_of_type(T data);
		int calculate_fletcher_checksum(const uint32_t* data_32, unsigned int data_32_count) const;
	};

	template<typename Enum> checksum& checksum::add_enum(Enum v) {
		return add_int(static_cast<int>(v));
	}

	template<typename Con, typename Pr> checksum& checksum::add_container(const Con& in, Pr getChecksum) {
		int i = 0;
		for (auto iter : in) {
			add_checksum_at_index(i, getChecksum(iter));
			i++;
		}
		return *this;
	}

	template<typename T> int checksum::calculate_fletcher_checksum_of_type(T data) {
		ASSERT(sizeof(T) % sizeof(unsigned int) == 0);
		int count = sizeof(T) / sizeof(unsigned int);
		return calculate_fletcher_checksum(reinterpret_cast<const unsigned int*>(&data), count);
	}

}

//http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
namespace std {
	template<>
	struct hash<solar::checksum>
	{
		typedef solar::checksum argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& s) const {
			return s.to_int();
		}
	};
}
