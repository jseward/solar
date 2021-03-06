#include "bitstream_archive_writer.h"

#include "archive_int_compression.h"
#include "solar/math/unit_convert.h"
#include "solar/utility/type_convert.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"
#include "solar/archiving/archivable.h"
#include "solar/colors/color.h"

namespace solar {

	bitstream_archive_writer::bitstream_archive_writer(unsigned char* preallocated_data, size_t preallocated_data_size) 
		: _preallocated_data(preallocated_data)
		, _preallocated_data_size(preallocated_data_size) 
		, _data(nullptr)
		, _data_bit_pos(0) 
		, _data_size_in_bits(0) {

		_data = _preallocated_data;
		_data_size_in_bits = bytes_to_bits(preallocated_data_size);
	}

	bitstream_archive_writer::~bitstream_archive_writer() {
		if (_data != _preallocated_data) {
			::free(_data);
		}
	}

	const unsigned char* bitstream_archive_writer::get_data() const {
		return _data;
	}

	size_t bitstream_archive_writer::get_data_size() const {
		return bits_to_bytes(_data_bit_pos);
	}

	void bitstream_archive_writer::begin_writing() {
	}

	void bitstream_archive_writer::end_writing() {
	}

	void bitstream_archive_writer::write_name(const char* name) {
		UNUSED_PARAMETER(name);
	}

	void bitstream_archive_writer::write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) {
		write_atomic_value<unsigned int>(size);
		for (unsigned int i = 0; i < size; ++i) {
			write_element_func(*this, i);
		}
	}

	void bitstream_archive_writer::write_object(std::function<void(archive_writer&)> write_object_func) {
		write_object_func(*this);
	}

	void bitstream_archive_writer::write_bool(bool value) {
		write_bit(value ? 1 : 0);
	}

	void bitstream_archive_writer::write_uint16(uint16_t value) {
		write_atomic_value<uint16_t>(value);
	}

	void bitstream_archive_writer::write_int(int value, const archive_int_compression& compression) {
		if (compression._type == archive_int_compression_type::NONE) {
			write_atomic_value<int>(value);
		}
		else if (compression._type == archive_int_compression_type::RANGE) {
			ASSERT(value >= compression._min_value);
			ASSERT(value <= compression._max_value);
			int relative_value = value - compression._min_value;
			write_bits(reinterpret_cast<const unsigned char*>(&relative_value), compression.get_bits_required());
		}
		else if (compression._type == archive_int_compression_type::SOFT_MAX_COUNT) {
			const bool is_within_max_count = (value <= compression._max_value);
			write_bool(is_within_max_count);
			if (is_within_max_count) {
				write_int(value, make_archive_int_compression_range(0, compression._max_value));
			}
			else {
				write_atomic_value<int>(value);
			}
		}
		else {
			ASSERT(false);
		}
	}

	void bitstream_archive_writer::write_int64(int64_t value) {
		write_atomic_value<int64_t>(value);
	}

	void bitstream_archive_writer::write_uint(unsigned int value) {
		write_atomic_value<unsigned int>(value);
	}

	void bitstream_archive_writer::write_float(float value) {
		write_atomic_value<float>(value);
	}

	void bitstream_archive_writer::write_string(const std::string& value) {
		uint16_t string_length = uint32_to_uint16(value.length());
		write_atomic_value<uint16_t>(string_length);
		for (unsigned short i = 0; i < string_length; ++i) {
			write_atomic_value<char>(value[i]);
		}
	}

	void bitstream_archive_writer::write_color(const color& value) {
		write_atomic_value<uint32_t>(value.to_argb32());
	}

	void bitstream_archive_writer::make_room_for_bits(unsigned int bit_count) {
		unsigned int needed_bit_count = _data_bit_pos + bit_count;
		if (needed_bit_count > _data_size_in_bits) {
			unsigned int extra_allocated_bit_count = std::min(needed_bit_count, 1048576U);
			unsigned int new_allocated_bit_count = needed_bit_count + extra_allocated_bit_count;

			if (_data == _preallocated_data) {
				TRACE("WARNING : bitstream_archive_writer malloc required. pre_allocated_size: {} , needed_size: {}", _preallocated_data_size, bits_to_bytes(new_allocated_bit_count));
				_data = static_cast<unsigned char*>(::malloc(bits_to_bytes(new_allocated_bit_count)));
				::memcpy_s(
					_data, bits_to_bytes(new_allocated_bit_count), 
					_preallocated_data, bits_to_bytes(_preallocated_data_size));
				_data_size_in_bits = new_allocated_bit_count;
			}
			else {
				_data = static_cast<unsigned char*>(::realloc(_data, bits_to_bytes(new_allocated_bit_count)));
				_data_size_in_bits = new_allocated_bit_count;
			}
		}
	}

	void bitstream_archive_writer::write_bit(unsigned char bit) {
		make_room_for_bits(1);

		unsigned int data_byte_index = _data_bit_pos >> 3;
		unsigned int data_bit_offset = _data_bit_pos % 8;
		if (data_bit_offset == 0) {
			if (bit == 1) {
				_data[data_byte_index] = 0x01;
			}
			else {
				_data[data_byte_index] = 0x00;
			}
		}
		else {
			if (bit == 1) {
				_data[data_byte_index] |= (0x01 << data_bit_offset);
			}
		}

		_data_bit_pos++;
	}

	void bitstream_archive_writer::write_bits(const unsigned char* bits, unsigned int bit_count) {
		make_room_for_bits(bit_count);

		unsigned int data_bit_offset = _data_bit_pos % 8;
		if ((data_bit_offset == 0) && ((bit_count % 8) == 0)) {
			//optimization, if both the source and destination are byte aligned can do a straight memcpy.
			//can safely >>3 to get byte index as we know we are byte aligned. no need for bits_to_bytes which rounds up.
			::memcpy_s(
				&_data[_data_bit_pos >> 3], bits_to_bytes(_data_size_in_bits) - (_data_bit_pos >> 3),
				bits, (bit_count >> 3));
			_data_bit_pos += bit_count;
		}
		else {
			unsigned int byte_to_write_index = 0;
			unsigned int remaining_bit_count_to_write = bit_count;
			while (remaining_bit_count_to_write > 0) {
				unsigned char byte_to_write = bits[byte_to_write_index];
				byte_to_write_index++;

				if (data_bit_offset == 0) {
					//destination is byte aligned, can do straight copy
					_data[_data_bit_pos >> 3] = byte_to_write;
				}
				else {
					//destination is not byte aligned, has to be done in two parts!
					_data[_data_bit_pos >> 3] |= (byte_to_write << data_bit_offset);
					
					unsigned int first_part_bit_count = (8 - data_bit_offset);
					if (remaining_bit_count_to_write > first_part_bit_count) {
						_data[(_data_bit_pos >> 3) + 1] = (byte_to_write >> first_part_bit_count);
					}
				}

				if (remaining_bit_count_to_write >= 8) {
					_data_bit_pos += 8;
					remaining_bit_count_to_write -= 8;
				}
				else {
					_data_bit_pos += remaining_bit_count_to_write;
					remaining_bit_count_to_write = 0;
				}
			}
		}
	}

}
