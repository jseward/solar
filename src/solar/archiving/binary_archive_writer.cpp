#include "binary_archive_writer.h"

#include "archivable.h"
#include "single_value_archivable.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	binary_archive_writer::binary_archive_writer(stream& stream)
		: _stream(stream) {
	}

	binary_archive_writer::~binary_archive_writer() {
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

	void binary_archive_writer::write_ushorts(const char* name, unsigned int size, std::function<unsigned short(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<unsigned short>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_ushorts(const char* name, unsigned int size, const unsigned short* values_begin) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<unsigned short>(values_begin[i]);
		}
	}

	void binary_archive_writer::write_int(const char* name, int value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<int>(value);
	}

	void binary_archive_writer::write_ints(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<int>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_ints(const char* name, unsigned int size, const int* values_begin) {
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

	void binary_archive_writer::write_uint(const char* name, unsigned int value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<unsigned int>(value);
	}

	void binary_archive_writer::write_float(const char* name, float value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<float>(value);
	}

	void binary_archive_writer::write_floats(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<float>(get_value_at_func(i));
		}
	}

	void binary_archive_writer::write_floats(const char* name, unsigned int size, const float* values_begin) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < size; ++i) {
			write_atomic_value<float>(values_begin[i]);
		}
	}

	void binary_archive_writer::write_string(const char* name, const std::string& value) {
		UNUSED_PARAMETER(name);
		write_atomic_value<int>(value.size());
		if (value.size() > 0) {
			_stream.write_bytes(value.c_str(), value.size());
		}
	}

}