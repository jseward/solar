#pragma once

#include <string>
#include <array>
#include <functional>

namespace solar {

	class archive_int_compression;
	class color;
	
	class archive_writer {
	public:
		virtual ~archive_writer() = 0 {};

		virtual void begin_writing() = 0;
		virtual void end_writing() = 0;

		virtual void write_name(const char* name) = 0;
		virtual void write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) = 0;
		virtual void write_object(std::function<void(archive_writer&)> write_object_func) = 0;
		virtual void write_bool(bool value) = 0;
		virtual void write_uint16(uint16_t value) = 0;
		virtual void write_int(int value, const archive_int_compression& compression) = 0;
		virtual void write_int64(int64_t value) = 0;
		virtual void write_uint(unsigned int value) = 0;
		virtual void write_float(float value) = 0;
		virtual void write_string(const std::string& value) = 0;
		virtual void write_color(const color& value) = 0;
	};

}
