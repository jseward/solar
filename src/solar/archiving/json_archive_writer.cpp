#include "json_archive_writer.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
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

	void json_archive_writer::write_ushorts_dynamic(const char* name, unsigned int size, std::function<unsigned short(unsigned int)> get_value_at_func) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_ushort(get_value_at_func(i));
		}
		_writer.end_array();
	}

	void json_archive_writer::write_ushorts_fixed(const char* name, unsigned int size, const unsigned short* values_begin) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_ushort(values_begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_int(const char* name, int value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(compression);
		ASSERT(_is_writing);
		_writer.write_int(name, value);
	}

	void json_archive_writer::write_ints_dynamic(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_int(get_value_at_func(i));
		}
		_writer.end_array();
	}

	void json_archive_writer::write_ints_fixed(const char* name, unsigned int size, const int* values_begin) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_int(values_begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_optional_int(const char* name, const optional<int>& value) {
		ASSERT(_is_writing);
		if (value.has_value()) {
			_writer.write_int(name, value.get_value());
		}
	}

	void json_archive_writer::write_int64(const char* name, int64_t value) {
		ASSERT(_is_writing);
		_writer.write_int64(name, value);
	}

	void json_archive_writer::write_uint(const char* name, unsigned int value) {
		ASSERT(_is_writing);
		_writer.write_uint(name, value);
	}

	void json_archive_writer::write_float(const char* name, float value) {
		ASSERT(_is_writing);
		_writer.write_float(name, value);
	}

	void json_archive_writer::write_floats_dynamic(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_float(get_value_at_func(i));
		}
		_writer.end_array();
	}

	void json_archive_writer::write_floats_fixed(const char* name, unsigned int size, const float* values_begin) {
		ASSERT(_is_writing);
		_writer.begin_array(name);
		for (unsigned int i = 0; i < size; ++i) {
			_writer.write_float(values_begin[i]);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_string(const char* name, const std::string& value) {
		ASSERT(_is_writing);
		_writer.write_string(name, value);
	}

}