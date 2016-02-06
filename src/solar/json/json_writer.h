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

		void write_name(const char* name);

		void begin_object();
		void end_object();

		void begin_array();
		void end_array();
		
		void write_bool(bool value);
		void write_uint16(uint16_t value);
		void write_int(int value);
		void write_int64(int64_t value);
		void write_uint(unsigned int value);
		void write_float(float value);
		void write_string(const std::string& value);
	};

}
