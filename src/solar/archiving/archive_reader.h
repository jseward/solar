#pragma once

#include <string>
#include <array>
#include <functional>

namespace solar {

	class archivable;
	class single_value_archivable;

	class archive_reader {

	public:
		virtual ~archive_reader() = 0 {};

		virtual std::string get_source_description() const = 0;
		virtual void raise_error(const std::string& error_message) = 0;

		virtual void read_object(const char* name, archivable& value) = 0;
		virtual void read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) = 0;
		virtual void read_bool(const char* name, bool& value) = 0;
		virtual void read_ushort(const char* name, unsigned short& value) = 0;		
		virtual void read_ushorts(const char* name, unsigned short* begin, unsigned int count) = 0;
		virtual void read_int(const char* name, int& value) = 0;
		virtual void read_ints(const char* name, int* begin, unsigned int count) = 0;
		virtual void read_uint(const char* name, unsigned int& value) = 0;
		virtual void read_float(const char* name, float& value) = 0;
		virtual void read_floats(const char* name, float* begin, unsigned int count) = 0;
		virtual void read_string(const char* name, std::string& value) = 0;
	};

}