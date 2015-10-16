#pragma once

#include <string>
#include <array>
#include <functional>
#include "solar/utility/optional.h"

namespace solar {

	class archivable;
	class single_value_archivable;
	class archive_int_compression;
	class color;
	
	class archive_reader {
	public:
		virtual ~archive_reader() = 0 {};

		virtual std::string get_source_description() const = 0;
		virtual void raise_error(const std::string& error_message) = 0;
		virtual unsigned int get_read_position() const = 0;

		virtual void read_object(const char* name, archivable& value) = 0;
		virtual void read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) = 0;
		virtual void read_bool(const char* name, bool& value) = 0;
		virtual void read_ushort(const char* name, unsigned short& value) = 0;
		virtual void read_ushorts_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, unsigned short)> handle_value_func) = 0;
		virtual void read_ushorts_fixed(const char* name, unsigned int size, unsigned short* values_begin) = 0;
		virtual void read_int(const char* name, int& value, const archive_int_compression& compression) = 0;
		virtual void read_ints_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, int)> handle_value_func) = 0;
		virtual void read_ints_fixed(const char* name, unsigned int size, int* values_begin) = 0;
		virtual void read_optional_int(const char* name, optional<int>& value) = 0;
		virtual void read_int64(const char* name, int64_t& value) = 0;
		virtual void read_uint(const char* name, unsigned int& value) = 0;
		virtual void read_float(const char* name, float& value) = 0;
		virtual void read_floats_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, float)> handle_value_func) = 0;
		virtual void read_floats_fixed(const char* name, unsigned int size, float* values_begin) = 0;
		virtual void read_string(const char* name, std::string& value) = 0;
		virtual void read_color(const char* name, color& value) = 0;
	};

}