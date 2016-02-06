#include "json_archive_writer.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/colors/color.h"
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

	void json_archive_writer::write_name(const char* name) {
		ASSERT(_is_writing);
		_writer.write_string(name);
	}

	void json_archive_writer::write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) {
		ASSERT(_is_writing);
		_writer.begin_array();
		for (unsigned int i = 0; i < size; ++i) {
			write_element_func(*this, i);
		}
		_writer.end_array();
	}

	void json_archive_writer::write_object(std::function<void(archive_writer&)> write_object_func) {
		ASSERT(_is_writing);
		_writer.begin_object();
		write_object_func(*this);
		_writer.end_object();
	}

	void json_archive_writer::write_bool(bool value) {
		ASSERT(_is_writing);
		_writer.write_bool(value);
	}

	void json_archive_writer::write_uint16(uint16_t value) {
		ASSERT(_is_writing);
		_writer.write_uint16(value);
	}

	void json_archive_writer::write_int(int value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(compression);
		ASSERT(_is_writing);
		_writer.write_int(value);
	}

	void json_archive_writer::write_int64(int64_t value) {
		ASSERT(_is_writing);
		_writer.write_int64(value);
	}

	void json_archive_writer::write_uint(unsigned int value) {
		ASSERT(_is_writing);
		_writer.write_uint(value);
	}

	void json_archive_writer::write_float(float value) {
		ASSERT(_is_writing);
		_writer.write_float(value);
	}

	void json_archive_writer::write_string(const std::string& value) {
		ASSERT(_is_writing);
		_writer.write_string(value);
	}

	void json_archive_writer::write_color(const color& value) {
		ASSERT(_is_writing);
		_writer.write_string(value.to_string());
	}

}