#include "binary_archive_reader.h"

#include <memory>
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/colors/color.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {

	binary_archive_reader::binary_archive_reader(stream& stream)
		: _stream(stream) {
	}

	binary_archive_reader::~binary_archive_reader() {
	}

	std::string binary_archive_reader::get_source_description() const {
		return _stream.get_description();
	}

	void binary_archive_reader::raise_error(const std::string& error_message) const {
		ALERT("binary_archive_reader error : {}\nstream : {}", error_message, _stream.get_description());
	}

	unsigned int binary_archive_reader::get_read_position() const {
		return 0; //not supported
	}

	void binary_archive_reader::read_name(const char* name) {
		UNUSED_PARAMETER(name); //todo - perhaps preserve for reporting?
	}

	void binary_archive_reader::read_array(
		std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
		std::function<void(archive_reader&, unsigned int)> read_element_func) {

		unsigned int size = read_atomic_value<unsigned int>();
		if (handle_size_func(*this, size)) {
			for (unsigned int i = 0; i < size; ++i) {
				read_element_func(*this, i);
			}
		}
	}

	void binary_archive_reader::read_object(std::function<void(archive_reader&)> read_object_func) {
		read_object_func(*this);
	}

	void binary_archive_reader::read_bool(bool& value) {
		value = read_atomic_value<bool>();
	}

	void binary_archive_reader::read_uint16(uint16_t& value) {
		value = read_atomic_value<uint16_t>();
	}

	void binary_archive_reader::read_int(int& value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(compression);
		value = read_atomic_value<int>();
	}

	void binary_archive_reader::read_int64(int64_t& value) {
		value = read_atomic_value<int64_t>();
	}

	void binary_archive_reader::read_uint(unsigned int& value) {
		value = read_atomic_value<unsigned int>();
	}

	void binary_archive_reader::read_float(float& value) {
		value = read_atomic_value<float>();
	}

	void binary_archive_reader::read_string(std::string& value) {
		unsigned int length = read_atomic_value<unsigned int>();
		if (length == 0) {
			value = "";
		}
		else {
			auto buffer = std::unique_ptr<char[]>(new char[length + 1]);
			VERIFY(_stream.read_bytes(buffer.get(), length) == length);
			buffer[length] = '\0';
			value = buffer.get();
		}
	}

	void binary_archive_reader::read_color(color& value) {
		uint32_t argb = read_atomic_value<uint32_t>();
		value = make_color_from_argb(argb);
	}

}