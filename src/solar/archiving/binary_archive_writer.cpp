#include "binary_archive_writer.h"

#include "archivable.h"
#include "single_value_archivable.h"
#include "solar/utility/unused_parameter.h"
#include "solar/colors/color.h"

namespace solar {

	binary_archive_writer::binary_archive_writer(stream& stream)
		: _stream(&stream) 
		, _should_calculate_size(should_calculate_size::NO)
		, _calculated_size(0) {
	}

	binary_archive_writer::binary_archive_writer(should_calculate_size should_calculate_size)
		: _stream(nullptr)
		, _should_calculate_size(should_calculate_size)
		, _calculated_size(0) {

		ASSERT(_should_calculate_size == should_calculate_size::YES);
	}

	binary_archive_writer::~binary_archive_writer() {
	}

	const unsigned int binary_archive_writer::get_calculated_size() const {
		ASSERT(_should_calculate_size == should_calculate_size::YES);
		return _calculated_size;
	}

	void binary_archive_writer::begin_writing() {
	}

	void binary_archive_writer::end_writing() {
	}

	void binary_archive_writer::write_name(const char* name) {
		UNUSED_PARAMETER(name);
	}

	void binary_archive_writer::write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) {
		write_atomic_value<unsigned int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_element_func(*this, i);
		}
	}

	void binary_archive_writer::write_object(std::function<void(archive_writer&)> write_object_func) {
		write_object_func(*this);
	}

	void binary_archive_writer::write_bool(bool value) {
		write_atomic_value<bool>(value);
	}

	void binary_archive_writer::write_uint16(uint16_t value) {
		write_atomic_value<uint16_t>(value);
	}

	void binary_archive_writer::write_int(int value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(compression);
		write_atomic_value<int>(value);
	}

	void binary_archive_writer::write_int64(int64_t value) {
		write_atomic_value<int64_t>(value);
	}

	void binary_archive_writer::write_uint(unsigned int value) {
		write_atomic_value<unsigned int>(value);
	}

	void binary_archive_writer::write_float(float value) {
		write_atomic_value<float>(value);
	}

	void binary_archive_writer::write_string(const std::string& value) {
		write_atomic_value<unsigned int>(value.size());
		if (value.size() > 0) {
			write_bytes(value.c_str(), value.size());
		}
	}

	void binary_archive_writer::write_color(const color& value) {
		write_atomic_value<uint32_t>(value.to_argb32());
	}

	void binary_archive_writer::write_bytes(const char* data, unsigned int data_size) {
		if (_should_calculate_size == should_calculate_size::YES) {
			_calculated_size += data_size;
		}
		else {
			_stream->write_bytes(data, data_size);
		}
	}

}