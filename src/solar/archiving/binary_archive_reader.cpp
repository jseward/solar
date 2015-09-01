#include "binary_archive_reader.h"

#include <memory>
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {
#include "archivable.h"

	binary_archive_reader::binary_archive_reader(stream& stream)
		: _stream(stream) {
	}

	binary_archive_reader::~binary_archive_reader() {
	}

	std::string binary_archive_reader::get_source_description() const {
		return _stream.get_description();
	}

	void binary_archive_reader::raise_error(const std::string& error_message) {
		ALERT("binary_archive_reader error : {}\nstream : {}", error_message, _stream.get_description());
	}

	void binary_archive_reader::read_object(const char* name, archivable& value) {
		UNUSED_PARAMETER(name);
		value.read_from_archive(*this);
	}

	void binary_archive_reader::read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) {
		UNUSED_PARAMETER(name);
		unsigned int size = 0;
		read_atomic_value<unsigned int>(size);
		handle_size_func(size);
		for (unsigned int i = 0; i < size; ++i) {
			read_object_func(*this, i);
		}
	}

	void binary_archive_reader::read_bool(const char* name, bool& value) {
		UNUSED_PARAMETER(name);
		read_atomic_value<bool>(value);
	}

	void binary_archive_reader::read_ushort(const char* name, unsigned short& value) {
		UNUSED_PARAMETER(name);
		read_atomic_value<unsigned short>(value);
	}

	void binary_archive_reader::read_ushorts(const char* name, unsigned short* begin, unsigned int count) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < count; ++i) {
			read_atomic_value<unsigned short>(begin[i]);
		}
	}

	void binary_archive_reader::read_int(const char* name, int& value) {
		UNUSED_PARAMETER(name);
		read_atomic_value<int>(value);
	}

	void binary_archive_reader::read_optional_int(const char* name, optional<int>& value) {
		UNUSED_PARAMETER(name);
		value.clear();

		bool has_int = false;
		read_atomic_value<bool>(has_int);
		if (has_int) {
			int read_value = 0;
			read_atomic_value<int>(read_value);
			value = read_value;
		}
	}

	void binary_archive_reader::read_ints(const char* name, int* begin, unsigned int count) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < count; ++i) {
			read_atomic_value<int>(begin[i]);
		}
	}

	void binary_archive_reader::read_uint(const char* name, unsigned int& value) {
		UNUSED_PARAMETER(name);
		read_atomic_value<unsigned int>(value);
	}

	void binary_archive_reader::read_float(const char* name, float& value) {
		UNUSED_PARAMETER(name);
		read_atomic_value<float>(value);
	}

	void binary_archive_reader::read_floats(const char* name, float* begin, unsigned int count) {
		UNUSED_PARAMETER(name);
		for (unsigned int i = 0; i < count; ++i) {
			read_atomic_value<float>(begin[i]);
		}
	}

	void binary_archive_reader::read_string(const char* name, std::string& value) {
		UNUSED_PARAMETER(name);

		unsigned int length = 0;
		read_atomic_value<unsigned int>(length);
		if (length <= 0) {
			value = "";
		}
		else {
			auto buffer = std::unique_ptr<char[]>(new char[length + 1]);
			VERIFY(_stream.read_bytes(buffer.get(), length) == length);
			buffer[length] = '\0';
			value = buffer.get();
		}
	}

}