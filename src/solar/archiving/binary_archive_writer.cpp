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

	void binary_archive_writer::write_object(const char* name, const archivable& value) {
		UNUSED_PARAMETER(name);
		value.write_to_archive(*this);
	}

	void binary_archive_writer::write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) {
		UNUSED_PARAMETER(name);
		write_atomic_value<int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_object_func(*this, i);
		}
	}

	void binary_archive_writer::write_bool(const char* name, bool value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<bool>(value);
	}

	void binary_archive_writer::write_ushort(const char* name, unsigned short value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned short>(value);
	}

	void binary_archive_writer::write_ushorts_dynamic(const char* name, unsigned int size, std::function<unsigned short(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<unsigned short>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_ushorts_fixed(const char* name, unsigned int size, const unsigned short* values_begin) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<unsigned short>(values_begin[i]);
		}
	}

	void binary_archive_writer::write_int(const char* name, int value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(compression);
		write_atomic_value<int>(value);
	}

	void binary_archive_writer::write_ints_dynamic(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<int>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_ints_fixed(const char* name, unsigned int size, const int* values_begin) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<int>(values_begin[i]);
		}
	}

	void binary_archive_writer::write_optional_int(const char* name, const optional<int>& value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<bool>(value.has_value());
		if (value.has_value()) {
			write_atomic_value<int>(value.get_value());
		}
	}

	void binary_archive_writer::write_int64(const char* name, int64_t value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<int64_t>(value);
	}

	void binary_archive_writer::write_uint(const char* name, unsigned int value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned int>(value);
	}

	void binary_archive_writer::write_float(const char* name, float value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<float>(value);
	}

	void binary_archive_writer::write_floats_dynamic(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<float>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_floats_fixed(const char* name, unsigned int size, const float* values_begin) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<float>(values_begin[i]);
		}
	}

	void binary_archive_writer::write_string(const char* name, const std::string& value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<int>(value.size());
		if (value.size() > 0) {
			write_bytes(value.c_str(), value.size());
		}
	}

	void binary_archive_writer::write_color(const char* name, const color& value) {
		UNUSED_PARAMETER(name);
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