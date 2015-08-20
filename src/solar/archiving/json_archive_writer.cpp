#include "json_archive_writer.h"

#include "solar/utility/assert.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {

	json_archive_writer::json_archive_writer(stream& stream)
		: _writer(stream)
		, _is_writing(false) {
	}

	json_archive_writer::~json_archive_writer() {
		ASSERT(!_is_writing);
	}

	void json_archive_writer::begin_writing() {
		ASSERT(!_is_writing);
		_is_writing = true;

		_writer.begin_object();
	}

	void json_archive_writer::end_writing() {
		ASSERT(_is_writing);
		_is_writing = false;

		_writer.end_object();
	}

	void json_archive_writer::write_object(const char* name, const archivable& value) {
		ASSERT(_is_writing);
		_writer.begin_object(name);
		value.write_to_archive(*this);
		_writer.end_object();
	}

	void json_archive_writer::write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.begin_object();
			write_object_func(*this, i);
			_writer.end_object();
		}
		_writer.end_array();
	}

	void json_archive_writer::write_bool(const char* name, bool value) {
		ASSERT(_is_writing);
		_writer.write_bool(name, value);
	}

	void json_archive_writer::write_ushort(const char* name, unsigned short value) {
		ASSERT(_is_writing);
		_writer.write_ushort(name, value);
	}

	void json_archive_writer::write_ushorts(const char* name, const unsigned short* begin, unsigned int count) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < count; ++i) {
			_writer.write_ushort(begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_int(const char* name, int value) {
		ASSERT(_is_writing);
		_writer.write_int(name, value);
	}

	void json_archive_writer::write_ints(const char* name, const int* begin, unsigned int count) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < count; ++i) {
			_writer.write_int(begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_uint(const char* name, unsigned int value) {
		ASSERT(_is_writing);
		_writer.write_uint(name, value);
	}

	void json_archive_writer::write_float(const char* name, float value) {
		ASSERT(_is_writing);
		_writer.write_float(name, value);
	}

	void json_archive_writer::write_floats(const char* name, const float* begin, unsigned int count) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < count; ++i) {
			_writer.write_float(begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_string(const char* name, const std::string& value) {
		ASSERT(_is_writing);
		_writer.write_string(name, value);
	}

}