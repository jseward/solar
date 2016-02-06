#pragma once

#include <string>
#include <vector>
#include "solar/strings/string_build.h"
#include "solar/strings/string_marshal.h"
#include "solar/utility/assert.h"
#include "solar/utility/enum_helpers.h"
#include "solar/utility/optional.h"
#include "archive_reader.h"
#include "archive_writer.h"
#include "archive_int_compression.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {

	namespace archiving_helpers {

		inline bool check_array_size_equal(archive_reader& reader, unsigned int expected_size, unsigned int actual_size) {
			if (actual_size != expected_size) {
				reader.raise_error(build_string("Array size not exact. expected:{} , actual:{}", expected_size, actual_size));
				return false;
			}
			return true;
		}

		inline bool check_array_size_less_than_or_equal(archive_reader& reader, unsigned int expected_size, unsigned int actual_size) {
			if (actual_size > expected_size) {
				reader.raise_error(build_string("Array size too large. max:{} , actual:{}", expected_size, actual_size));
				return false;
			}
			return true;
		}

	}

	inline void read_bool(archive_reader& reader, const char* name, bool& value) {
		reader.read_name(name);
		reader.read_bool(value);
	}

	inline void write_bool(archive_writer& writer, const char* name, bool value) {
		writer.write_name(name);
		writer.write_bool(value);
	}

	inline void read_uint16(archive_reader& reader, const char* name, uint16_t& value) {
		reader.read_name(name);
		reader.read_uint16(value);
	}

	inline void write_uint16(archive_writer& writer, const char* name, uint16_t value) {
		writer.write_name(name);
		writer.write_uint16(value);
	}

	inline void read_uint16s(archive_reader& reader, const char* name, uint16_t& v0, uint16_t& v1, uint16_t& v2) {
		reader.read_name(name);
		uint16_t values[3] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 3, size); 
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_uint16(values[index]); 
			});
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
	}

	inline void write_uint16s(archive_writer& writer, const char* name, uint16_t v0, uint16_t v1, uint16_t v2) {
		writer.write_name(name);
		const uint16_t values[3] = { v0, v1, v2 };
		writer.write_array(
			3, 
			[&values](archive_writer& writer, unsigned int index) { 
				writer.write_uint16(values[index]); 
			});
	}

	template<typename VectorT> void read_uint16_vector(archive_reader& reader, const char* name, VectorT& v) {
		reader.read_name(name);
		static_assert(sizeof(VectorT::value_type) == sizeof(uint16_t), "read_uint16_vector expects uint16");
		reader.read_array(
			[&v](archive_reader&, unsigned int size) { 
				v.clear();
				v.reserve(size);
				return true;
			},
			[&v](archive_reader& reader, unsigned int) { 
				uint16_t read_value;
				reader.read_uint16(read_value);
				v.push_back(read_value); 
			});
	}

	template<typename VectorT> 
	void write_uint16_vector(archive_writer& writer, const char* name, const VectorT& v) {
		writer.write_name(name);
		writer.write_array(
			v.size(), 
			[&v](archive_writer& writer, unsigned int i) { 
				writer.write_uint16(v[i]); 
			});
	}

	inline void read_int(archive_reader& reader, const char* name, int& value) {
		reader.read_name(name);
		reader.read_int(value, archive_int_compression());
	}

	inline void write_int(archive_writer& writer, const char* name, int value) {
		writer.write_name(name);
		writer.write_int(value, archive_int_compression());
	}

	inline void read_int(archive_reader& reader, const char* name, int& value, const archive_int_compression& compression) {
		reader.read_name(name);
		reader.read_int(value, compression);
	}

	inline void write_int(archive_writer& writer, const char* name, int value, const archive_int_compression& compression) {
		writer.write_name(name);
		writer.write_int(value, compression);
	}

	inline void read_optional_int(archive_reader& reader, const char* has_value_name, const char* value_name, optional<int>& value) {
		value.clear();

		reader.read_name(has_value_name);
		bool has_value = false;
		reader.read_bool(has_value);
		if (has_value) {
			reader.read_name(value_name);
			int read_value = 0;
			reader.read_int(read_value, archive_int_compression());
			value = read_value;
		}
	}

	inline void write_optional_int(archive_writer& writer, const char* has_value_name, const char* value_name, const optional<int>& value) {
		writer.write_name(has_value_name);
		writer.write_bool(value.has_value());
		if (value.has_value()) {
			writer.write_name(value_name);
			writer.write_int(value.get_value(), archive_int_compression());
		}
	}

	inline void write_int64(archive_writer& writer, const char* name, int64_t value) {
		writer.write_name(name);
		writer.write_int64(value);
	}

	inline void read_int64(archive_reader& reader, const char* name, int64_t& value) {
		reader.read_name(name);
		reader.read_int64(value);
	}

	inline void read_ints(archive_reader& reader, const char* name, int& v0, int& v1) {
		reader.read_name(name);
		int values[2] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 2, size); 
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_int(values[index], archive_int_compression()); 
			});
		v0 = values[0];
		v1 = values[1];
	}

	inline void write_ints(archive_writer& writer, const char* name, int v0, int v1) {
		writer.write_name(name);
		const int values[2] = { v0, v1 };
		writer.write_array(2, [&values](archive_writer& writer, unsigned int index) { writer.write_int(values[index], archive_int_compression()); });
	}

	inline void read_ints(archive_reader& reader, const char* name, int& v0, int& v1, int& v2, int& v3) {
		reader.read_name(name);
		int values[4] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 4, size);
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_int(values[index], archive_int_compression()); 
			});
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
		v3 = values[3];
	}

	inline void write_ints(archive_writer& writer, const char* name, int v0, int v1, int v2, int v3) {
		writer.write_name(name);
		const int values[4] = { v0, v1, v2, v3 };
		writer.write_array(
			4, 
			[&values](archive_writer& writer, unsigned int index) { 
				writer.write_int(values[index], archive_int_compression()); });
	}

	inline void read_uint(archive_reader& reader, const char* name, unsigned int& value) {
		reader.read_name(name);
		reader.read_uint(value);
	}

	inline void write_uint(archive_writer& writer, const char* name, unsigned int value) {
		writer.write_name(name);
		writer.write_uint(value);
	}

	inline void read_float(archive_reader& reader, const char* name, float& value) {
		reader.read_name(name);
		reader.read_float(value);
	}

	inline void write_float(archive_writer& writer, const char* name, float value) {
		writer.write_name(name);
		writer.write_float(value);
	}

	inline void read_floats(archive_reader& reader, const char* name, float& v0, float& v1) {
		if (name != nullptr) {
			reader.read_name(name);
		}

		float values[2] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 2, size); 
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_float(values[index]); 
			});
		v0 = values[0];
		v1 = values[1];
	}

	inline void write_floats(archive_writer& writer, const char* name, float v0, float v1) {
		if (name != nullptr) {
			writer.write_name(name);
		}

		const float values[2] = { v0, v1 };
		writer.write_array(
			2, 
			[&values](archive_writer& writer, unsigned int index) {
				writer.write_float(values[index]);
			});
	}

	inline void read_floats(archive_reader& reader, const char* name, float& v0, float& v1, float& v2) {
		reader.read_name(name);
		float values[3] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 3, size); 
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_float(values[index]); 
			});
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
	}

	inline void write_floats(archive_writer& writer, const char* name, float v0, float v1, float v2) {
		writer.write_name(name);
		const float values[3] = { v0, v1, v2 };
		writer.write_array(
			3,
			[&values](archive_writer& writer, unsigned int index) {
			writer.write_float(values[index]);
		});
	}

	inline void read_floats(archive_reader& reader, const char* name, float& v0, float& v1, float& v2, float& v3) {
		reader.read_name(name);
		float values[4] = {};
		reader.read_array(
			[](archive_reader& reader, unsigned int size) { 
				return archiving_helpers::check_array_size_equal(reader, 4, size); 
			},
			[&values](archive_reader& reader, unsigned int index) { 
				reader.read_float(values[index]); 
			});
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
		v3 = values[3];
	}

	inline void write_floats(archive_writer& writer, const char* name, float v0, float v1, float v2, float v3) {
		writer.write_name(name);
		const float values[4] = { v0, v1, v2, v3 };
		writer.write_array(
			4,
			[&values](archive_writer& writer, unsigned int index) {
			writer.write_float(values[index]);
		});
	}

	inline void read_string(archive_reader& reader, const char* name, std::string& value) {
		reader.read_name(name);
		reader.read_string(value);
	}

	inline void write_string(archive_writer& writer, const char* name, const std::string& value) {
		writer.write_name(name);
		writer.write_string(value);
	}

	template<typename VectorT> void read_string_vector(archive_reader& reader, const char* name, VectorT& values) {
		reader.read_name(name);
		reader.read_array(
			[&values](archive_reader&, unsigned int size) {
				values.clear();
				values.reserve(size);
				return true;
			},
			[&values](archive_reader& reader, unsigned int) {
				std::string read_value;
				reader.read_string(read_value);
				values.emplace_back(read_value);
			});
	}

	template<typename VectorT> void write_string_vector(archive_writer& writer, const char* name, const VectorT& values) {
		writer.write_name(name);
		auto iter = values.begin();
		writer.write_array(
			values.size(),
			[&iter](archive_writer& writer, unsigned int) {
				writer.write_string(*iter);
				iter++;
			});
	}

	inline void read_wstring(archive_reader& reader, const char* name, std::wstring& value) {
		reader.read_name(name);
		std::string utf8_value;
		reader.read_string(utf8_value);
		value = utf8_to_utf16(utf8_value);
	}

	inline void write_wstring(archive_writer& writer, const char* name, const std::wstring& value) {
		writer.write_name(name);
		auto utf8_value = utf16_to_utf8(value);
		writer.write_string(utf8_value);
	}

	inline void write_color(archive_writer& writer, const char* name, const color& value) {
		writer.write_name(name);
		writer.write_color(value);
	}

	inline void read_color(archive_reader& reader, const char* name, color& value) {
		reader.read_name(name);
		reader.read_color(value);
	}

	inline void read_object(archive_reader& reader, const char* name, archivable& value) {
		if (name != nullptr) {
			reader.read_name(name);
		}
		reader.read_object([&value](archive_reader& reader) { value.read_from_archive(reader); });
	}

	inline void write_object(archive_writer& writer, const char* name, const archivable& value) {
		if (name != nullptr) {
			writer.write_name(name);
		}
		writer.write_object([&value](archive_writer& writer) { value.write_to_archive(writer); });
	}

	inline void read_object(archive_reader& reader, const char* name, single_value_archivable& value) {
		value.read_from_archive(reader, name);
	}

	inline void write_object(archive_writer& writer, const char* name, const single_value_archivable& value) {
		value.write_to_archive(writer, name);
	}

	inline void read_object_if(archive_reader& reader, const char* name, single_value_archivable& value, const char* condition_name) {
		bool should_read = false;
		read_bool(reader, condition_name, should_read);
		if (should_read) {
			value.read_from_archive(reader, name);
		}
	}

	inline void write_object_if(archive_writer& writer, const char* name, const single_value_archivable& value, const char* condition_name, bool should_write) {
		write_bool(writer, condition_name, should_write);
		if (should_write) {
			value.write_to_archive(writer, name);
		}
	}

	template<typename VectorT> void read_object_vector(archive_reader& reader, const char* name, VectorT& values) {
		reader.read_name(name);
		reader.read_array(
			[&values](archive_reader&, unsigned int size) { 
				values.clear();
				values.reserve(size);
				return true;
			},
			[&values](archive_reader& reader, unsigned int) { 
				VectorT::value_type read_value;
				reader.read_object([&read_value](archive_reader& reader) { read_value.read_from_archive(reader); });
				values.emplace_back(read_value);
			});
	}

	template<typename VectorT> void write_object_vector(archive_writer& writer, const char* name, const VectorT& values) {
		writer.write_name(name);
		auto iter = values.begin();
		writer.write_array(
			values.size(),
			[&iter](archive_writer& writer, unsigned int) {
				writer.write_object([&iter](archive_writer& writer) { iter->write_to_archive(writer); });
				iter++;
			});
	}

	template<typename FixedVectorT> void read_object_fixed_vector(archive_reader& reader, const char* name, FixedVectorT& values) {
		reader.read_name(name);		
		reader.read_array(
			[&values](archive_reader& reader, unsigned int size) { 
				values.clear();
				return archiving_helpers::check_array_size_less_than_or_equal(reader, FixedVectorT::MAX_SIZE, size);
			},
			[&values](archive_reader& reader, unsigned int) {
				FixedVectorT::value_type read_value;
				reader.read_object([&read_value](archive_reader& reader) { read_value.read_from_archive(reader); });
				values.push_back(read_value);
			});
	}

	template<typename FixedVectorT> void write_object_fixed_vector(archive_writer& writer, const char* name, const FixedVectorT& values) {
		writer.write_name(name);
		writer.write_array(
			values.size(),
			[&values](archive_writer& writer, unsigned int index) {
				auto& write_value = values[index];
				writer.write_object([&write_value](archive_writer& writer) { write_value.write_to_archive(writer); });
			});
	}

}