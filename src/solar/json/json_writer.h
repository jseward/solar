#pragma once

#include <memory>
#include "solar/json/internal/rapidjson_headers.h"
#include "solar/json/internal/rapidjson_stream.h"

namespace solar {

	class json_writer {
	private:
		rapidjson_stream _json_stream;
		rapidjson::PrettyWriter<rapidjson_stream> _writer;

	public:
		json_writer(stream& stream);

		void begin_object();
		void begin_object(const char* name);
		void end_object();

		void begin_array();
		void begin_array(const char* name);
		void end_array();
		
		void write_bool(const char* name, bool value);
		void write_uint16(const char* name, uint16_t value);
		void write_uint16(uint16_t value);
		void write_int(const char* name, int value);
		void write_int(int value);
		void write_int64(const char* name, int64_t value);
		void write_uint(const char* name, unsigned int value);
		void write_float(const char* name, float value);
		void write_float(float value);
		void write_string(const char* name, const std::string& value);
	};

}
