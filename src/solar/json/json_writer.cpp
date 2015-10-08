#include "json_writer.h"

namespace solar {

	json_writer::json_writer(stream& stream) 
		: _json_stream(stream)
		, _writer(_json_stream) {
	}

	void json_writer::begin_object() {
		_writer.StartObject();
	}

	void json_writer::begin_object(const char* name) {
		_writer.String(name);
		_writer.StartObject();
	}

	void json_writer::end_object() {
		_writer.EndObject();
	}

	void json_writer::begin_array() {
		_writer.StartArray();
	}

	void json_writer::begin_array(const char* name) {
		_writer.String(name);
		_writer.StartArray();
	}

	void json_writer::end_array() {
		_writer.EndArray();
	}

	void json_writer::write_bool(const char* name, bool value) {
		_writer.String(name);
		_writer.Bool(value);
	}

	void json_writer::write_ushort(const char* name, unsigned short value) {
		_writer.String(name);
		_writer.Uint(value);
	}

	void json_writer::write_ushort(unsigned short value) {
		_writer.Uint(value);
	}

	void json_writer::write_int(const char* name, int value) {
		_writer.String(name);
		_writer.Int(value);
	}

	void json_writer::write_int(int value) {
		_writer.Int(value);
	}

	void json_writer::write_int64(const char* name, int64_t value) {
		_writer.String(name);
		_writer.Int64(value);
	}

	void json_writer::write_uint(const char* name, unsigned int value) {
		_writer.String(name);
		_writer.Uint(value);
	}

	void json_writer::write_float(const char* name, float value) {
		_writer.String(name);
		_writer.Double(value);
	}

	void json_writer::write_float(float value) {
		_writer.Double(value);
	}

	void json_writer::write_string(const char* name, const std::string& value) {
		_writer.String(name);
		_writer.String(value.c_str());
	}

}