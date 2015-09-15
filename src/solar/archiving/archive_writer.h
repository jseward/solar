#pragma once

#include <string>
#include <array>
#include <functional>
#include "solar/utility/optional.h"

namespace solar {

	class archivable;
	class single_value_archivable;

	class archive_writer {

	public:
		virtual ~archive_writer() = 0 {};

		virtual void begin_writing() = 0;
		virtual void end_writing() = 0;
		virtual void write_object(const char* name, const archivable& value) = 0;
		virtual void write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) = 0;
		virtual void write_bool(const char* name, bool value) = 0;
		virtual void write_ushort(const char* name, unsigned short value) = 0;
		virtual void write_ushorts(const char* name, unsigned int size, std::function<unsigned short(unsigned int)> get_value_at_func) = 0;
		virtual void write_ushorts(const char* name, unsigned int size, const unsigned short* values_begin) = 0;
		virtual void write_int(const char* name, int value) = 0;
		virtual void write_ints(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) = 0;
		virtual void write_ints(const char* name, unsigned int size, const int* values_begin) = 0;
		virtual void write_optional_int(const char* name, const optional<int>& value) = 0;
		virtual void write_uint(const char* name, unsigned int value) = 0;
		virtual void write_float(const char* name, float value) = 0;
		virtual void write_floats(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) = 0;
		virtual void write_floats(const char* name, unsigned int size, const float* values_begin) = 0;
		virtual void write_string(const char* name, const std::string& value) = 0;
	};

}
