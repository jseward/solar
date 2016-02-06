#include "json_writer.h"

namespace solar {

	json_writer::json_writer(stream& stream) 
		: _json_stream(stream)
		, _writer(_json_stream) {
	}

	void json_writer::write_name(const char* name) {
		_writer.String(name);
	}

	void json_writer::begin_object() {
		_writer.StartObject();
	}

	void json_writer::end_object() {
		_writer.EndObject();
	}

	void json_writer::begin_array() {
		_writer.StartArray();
	}

	void json_writer::end_array() {
		_writer.EndArray();
	}

	void json_writer::write_bool(bool value) {
		_writer.Bool(value);
	}

	void json_writer::write_uint16(uint16_t value) {
		_writer.Uint(value);
	}

	void json_writer::write_int(int value) {
		_writer.Int(value);
	}

	void json_writer::write_int64(int64_t value) {
		_writer.Int64(value);
	}

	void json_writer::write_uint(unsigned int value) {
		_writer.Uint(value);
	}

	void json_writer::write_float(float value) {
		_writer.Double(value);
	}

	void json_writer::write_string(const std::string& value) {
		_writer.String(value.c_str());
	}

}
