#pragma once

#include <string>
#include <vector>
#include "solar/strings/string_build.h"
#include "solar/strings/string_marshal.h"
#include "solar/utility/assert.h"
#include "solar/utility/enum_helpers.h"
#include "archive_reader.h"
#include "archive_writer.h"
#include "archive_int_compression.h"
#include "single_value_archivable.h"

namespace solar {

	inline void read_bool(archive_reader& reader, const char* name, bool& value) {
		reader.read_bool(name, value);
	}

	inline void write_bool(archive_writer& writer, const char* name, bool value) {
		writer.write_bool(name, value);
	}

	inline void read_ushort(archive_reader& reader, const char* name, unsigned short& value) {
		reader.read_ushort(name, value);
	}

	inline void write_ushort(archive_writer& writer, const char* name, unsigned short value) {
		writer.write_ushort(name, value);
	}

	inline void read_ushorts(archive_reader& reader, const char* name, unsigned short& v0, unsigned short& v1, unsigned short& v2) {
		unsigned short values[3] = {};
		reader.read_ushorts_fixed(name, 3, values);
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
	}

	inline void write_ushorts(archive_writer& writer, const char* name, unsigned short v0, unsigned short v1, unsigned short v2) {
		const unsigned short values[3] = { v0, v1, v2 };
		writer.write_ushorts_fixed(name, 3, values);
	}

	template<typename VectorT> void read_ushort_vector(archive_reader& reader, const char* name, VectorT& v) {
		v.clear();
		reader.read_ushorts_dynamic(
			name,
			[&v](unsigned int size) { v.reserve(size); },
			[&v](unsigned int, unsigned short value) { v.push_back(value); });
	}

	template<typename VectorT> 
	void write_ushort_vector(archive_writer& writer, const char* name, const VectorT& v) {
		writer.write_ushorts_dynamic(name, v.size(), [&v](unsigned int i) { return v[i]; });
	}

	inline void read_int(archive_reader& reader, const char* name, int& value) {
		reader.read_int(name, value, archive_int_compression());
	}

	inline void write_int(archive_writer& writer, const char* name, int value) {
		writer.write_int(name, value, archive_int_compression());
	}

	inline void read_int(archive_reader& reader, const char* name, int& value, const archive_int_compression& compression) {
		reader.read_int(name, value, compression);
	}

	inline void write_int(archive_writer& writer, const char* name, int value, const archive_int_compression& compression) {
		writer.write_int(name, value, compression);
	}

	inline void read_optional_int(archive_reader& reader, const char* name, optional<int>& value) {
		reader.read_optional_int(name, value);
	}

	inline void write_optional_int(archive_writer& writer, const char* name, const optional<int>& value) {
		writer.write_optional_int(name, value);
	}

	inline void write_int64(archive_writer& writer, const char* name, int64_t value) {
		writer.write_int64(name, value);
	}

	inline void read_int64(archive_reader& reader, const char* name, int64_t& value) {
		reader.read_int64(name, value);
	}

	inline void read_ints(archive_reader& reader, const char* name, int& v0, int& v1) {
		int values[2] = {};
		reader.read_ints_fixed(name, 2, values);
		v0 = values[0];
		v1 = values[1];
	}

	inline void write_ints(archive_writer& writer, const char* name, int v0, int v1) {
		const int values[2] = { v0, v1 };
		writer.write_ints_fixed(name, 2, values);
	}

	inline void read_ints(archive_reader& reader, const char* name, int& v0, int& v1, int& v2, int& v3) {
		int values[4] = {};
		reader.read_ints_fixed(name, 4, values);
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
		v3 = values[3];
	}

	inline void write_ints(archive_writer& writer, const char* name, int v0, int v1, int v2, int v3) {
		const int values[4] = { v0, v1, v2, v3 };
		writer.write_ints_fixed(name, 4, values);
	}

	inline void read_uint(archive_reader& reader, const char* name, unsigned int& value) {
		reader.read_uint(name, value);
	}

	inline void write_uint(archive_writer& writer, const char* name, unsigned int value) {
		writer.write_uint(name, value);
	}

	inline void read_float(archive_reader& reader, const char* name, float& value) {
		reader.read_float(name, value);
	}

	inline void write_float(archive_writer& writer, const char* name, float value) {
		writer.write_float(name, value);
	}

	inline void read_floats(archive_reader& reader, const char* name, float& v0, float& v1) {
		float values[2] = {};
		reader.read_floats_fixed(name, 2, values);
		v0 = values[0];
		v1 = values[1];
	}

	inline void write_floats(archive_writer& writer, const char* name, float v0, float v1) {
		const float values[2] = { v0, v1 };
		writer.write_floats_fixed(name, 2, values);
	}

	inline void read_floats(archive_reader& reader, const char* name, float& v0, float& v1, float& v2) {
		float values[3] = {};
		reader.read_floats_fixed(name, 3, values);
		v0 = values[0];
		v1 = values[1];
		v2 = values[2];
	}

	inline void write_floats(archive_writer& writer, const char* name, float v0, float v1, float v2) {
		const float values[3] = { v0, v1, v2 };
		writer.write_floats_fixed(name, 3, values);
	}

	inline void read_string(archive_reader& reader, const char* name, std::string& value) {
		reader.read_string(name, value);
	}

	inline void write_string(archive_writer& writer, const char* name, const std::string& value) {
		writer.write_string(name, value);
	}

	inline void read_string(archive_reader& reader, const char* name, std::wstring& value) {
		std::string utf8_value;
		reader.read_string(name, utf8_value);
		value = utf8_to_utf16(utf8_value);
	}

	inline void write_string(archive_writer& writer, const char* name, const std::wstring& value) {
		auto utf8_value = utf16_to_utf8(value);
		writer.write_string(name, utf8_value);
	}

	inline void write_color(archive_writer& writer, const char* name, const color& value) {
		writer.write_color(name, value);
	}

	inline void read_color(archive_reader& reader, const char* name, color& value) {
		reader.read_color(name, value);
	}

	inline void read_object(archive_reader& reader, const char* name, archivable& value) {
		reader.read_object(name, value);
	}

	inline void write_object(archive_writer& writer, const char* name, const archivable& value) {
		writer.write_object(name, value);
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

	template<typename VectorT> void read_object_vector(archive_reader& reader, const char* name, VectorT& values, std::function<typename VectorT::value_type(archive_reader&)> read_value_func) {
		values.clear();
		reader.read_objects(
			name,
			[&](int size) { values.reserve(size); },
			[&](archive_reader& reader, unsigned int) { values.push_back(read_value_func(reader)); });
	}

	template<typename VectorT> void read_object_vector(archive_reader& reader, const char* name, VectorT& values) {
		read_object_vector(reader, name, values, [](archive_reader& reader) {
			VectorT::value_type value;
			value.read_from_archive(reader);
			return value;
		});
	}

	template<typename VectorT> void write_object_vector(archive_writer& writer, const char* name, const VectorT& values, std::function<void(archive_writer&, typename const VectorT::value_type&)> write_value_func) {
		auto iter = values.begin();
		writer.write_objects(
			name,
			values.size(),
			[&](archive_writer& writer, unsigned int) {
				write_value_func(writer, *iter);
				iter++;
			});
	}

	template<typename VectorT> void write_object_vector(archive_writer& writer, const char* name, const VectorT& values) {
		write_object_vector(writer, name, values, [](archive_writer& writer, typename const VectorT::value_type& value) {
			value.write_to_archive(writer);
		});
	}

	template<typename DequeT> void read_object_deque(archive_reader& reader, const char* name, DequeT& values, std::function<typename DequeT::value_type(archive_reader&)> read_value_func) {
		values.clear();
		reader.read_objects(
			name,
			[](int) {},
			[&](archive_reader& reader, unsigned int) { values.push_back(read_value_func(reader)); });
	}

	template<typename DequeT> void read_object_deque(archive_reader& reader, const char* name, DequeT& values) {
		read_object_deque(reader, name, values, [](archive_reader& reader) {
			DequeT::value_type value;
			value.read_from_archive(reader);
			return value;
		});
	}

	template<typename DequeT> void write_object_deque(archive_writer& writer, const char* name, const DequeT& values, std::function<void(archive_writer&, typename const DequeT::value_type&)> write_value_func) {
		auto iter = values.begin();
		writer.write_objects(
			name,
			values.size(),
			[&](archive_writer& writer, unsigned int) {
				write_value_func(writer, *iter);
				iter++;
			});
	}

	template<typename DequeT> void write_object_deque(archive_writer& writer, const char* name, const DequeT& values) {
		write_object_deque(writer, name, values, [](archive_writer& writer, typename const DequeT::value_type& value) {
			value.write_to_archive(writer);
		});
	}

	template<typename FixedVectorT> void read_object_fixed_vector(archive_reader& reader, const char* name, FixedVectorT& values) {
		values.clear();
		reader.read_objects(
			name,
			[&](int size) {
				if (size > FixedVectorT::MAX_SIZE) {
					reader.raise_error(build_string("max elements exceeded ({} > {}) : {}", size, FixedVectorT::MAX_SIZE, name));
				}
			},
			[&](archive_reader& reader, unsigned int) {
				FixedVectorT::value_type value;
				value.read_from_archive(reader);
				if (!values.full()) {
					values.push_back(value);
				}
			});
	}

	template<typename FixedVectorT> void write_object_fixed_vector(archive_writer& writer, const char* name, const FixedVectorT& values) {
		auto iter = values.begin();
		writer.write_objects(
			name,
			values.size(),
			[&](archive_writer& writer, unsigned int) {
				iter->write_to_archive(writer);
				iter++;
			});
	}

}