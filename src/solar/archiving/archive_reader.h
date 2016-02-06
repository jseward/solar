#pragma once

#include <string>
#include <array>
#include <functional>

namespace solar {

	class archive_int_compression;
	class color;
	
	class archive_reader {
	public:
		virtual ~archive_reader() = 0 {};

		virtual std::string get_source_description() const = 0;
		virtual void raise_error(const std::string& error_message) const = 0;
		virtual unsigned int get_read_position() const = 0;

		virtual void read_name(const char* name) = 0;
		virtual void read_array(
			std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
			std::function<void(archive_reader&, unsigned int)> read_element_func) = 0;
		virtual void read_object(std::function<void(archive_reader&)> read_object_func) = 0;
		virtual void read_bool(bool& value) = 0;
		virtual void read_uint16(uint16_t& value) = 0;
		virtual void read_int(int& value, const archive_int_compression& compression) = 0;
		virtual void read_int64(int64_t& value) = 0;
		virtual void read_uint(unsigned int& value) = 0;
		virtual void read_float(float& value) = 0;
		virtual void read_string(std::string& value) = 0;
		virtual void read_color(color& value) = 0;
	};

}