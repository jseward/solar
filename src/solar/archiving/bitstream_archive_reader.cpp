#include "bitstream_archive_reader.h"

#include "archivable.h"
#include "archive_int_compression.h"
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/math/unit_convert.h"
#include "solar/colors/color.h"

namespace solar {

	bitstream_archive_reader::bitstream_archive_reader(const unsigned char* data, size_t data_size)
		: _data(data)
		, _data_size(data_size) 
		, _data_size_in_bits(bytes_to_bits(data_size)) 
		, _data_bit_pos(0) {
	}

	bitstream_archive_reader::~bitstream_archive_reader() {
	}

	std::string bitstream_archive_reader::get_source_description() const {
		return "bitstream";
	}

	void bitstream_archive_reader::raise_error(const std::string& error_message) const {
		ALERT("bitstream_archive_reader error : {}", error_message);
	}

	unsigned int bitstream_archive_reader::get_read_position() const {
		return bits_to_bytes(_data_bit_pos);
	}

	void bitstream_archive_reader::read_name(const char* name) {
		UNUSED_PARAMETER(name);
	}

	void bitstream_archive_reader::read_array(
		std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
		std::function<void(archive_reader&, unsigned int)> read_element_func) {

		auto size = read_atomic_value<unsigned int>();
		if (handle_size_func(*this, size)) {
			for (unsigned int i = 0; i < size; ++i) {
				read_element_func(*this, i);
			}
		}
	}

	void bitstream_archive_reader::read_object(std::function<void(archive_reader&)> read_object_func) {
		read_object_func(*this);
	}

	void bitstream_archive_reader::read_bool(bool& value) {
		unsigned char value_as_char = 0;
		read_bit(&value_as_char);
		value = (value_as_char == 1) ? true : false;
	}

	void bitstream_archive_reader::read_uint16(uint16_t& value) {
		value = read_atomic_value<uint16_t>();
	}

	void bitstream_archive_reader::read_int(int& value, const archive_int_compression& compression) {
		if (compression._type == archive_int_compression_type::NONE) {
			value = read_atomic_value<int>();
		}
		else if (compression._type == archive_int_compression_type::RANGE) {
			int relative_value = 0;
			read_bits(reinterpret_cast<unsigned char*>(&relative_value), compression.get_bits_required());
			value = compression._min_value + relative_value;
		}
		else if (compression._type == archive_int_compression_type::SOFT_MAX_COUNT) {
			bool is_within_max_count = false;
			read_bool(is_within_max_count);
			if (is_within_max_count) {
				read_int(value, make_archive_int_compression_range(0, compression._max_value));
			}
			else {
				value = read_atomic_value<int>();
			}
		}
		else {
			ASSERT(false);
		}
	}

	void bitstream_archive_reader::read_int64(int64_t& value) {
		value = read_atomic_value<int64_t>();
	}

	void bitstream_archive_reader::read_uint(unsigned int& value) {
		value = read_atomic_value<unsigned int>();
	}

	void bitstream_archive_reader::read_float(float& value) {
		value = read_atomic_value<float>();
	}

	void bitstream_archive_reader::read_string(std::string& value) {
		uint16_t string_length = read_atomic_value<uint16_t>();
		value = std::string(string_length, '?');
		for (uint16_t i = 0; i < string_length; ++i) {
			value[i] = read_atomic_value<char>();
		}
	}

	void bitstream_archive_reader::read_color(color& value) {
		uint32_t argb = read_atomic_value<uint32_t>();
		value = make_color_from_argb(argb);
	}

	bool bitstream_archive_reader::read_bit(unsigned char* bit) {
		unsigned int data_byte_index = _data_bit_pos >> 3;
		if (data_byte_index >= _data_size) {
			raise_error("no more bits available to read");
			return false;
		}

		unsigned int data_bit_offset = _data_bit_pos % 8;
		if ((_data[data_byte_index] & (0x01 << data_bit_offset)) != 0) {
			*bit = 1;
		}
		else {
			*bit = 0;
		}

		_data_bit_pos++;
		return true;
	}

	bool bitstream_archive_reader::read_bits(unsigned char* bits, unsigned int bit_count) {
		if ((_data_bit_pos + bit_count) > _data_size_in_bits) {
			raise_error("no more bits available to read");
			return false;
		}

		unsigned int data_bit_offset = _data_bit_pos % 8;
		if ((data_bit_offset == 0) && ((bit_count % 8) == 0)) {
			//optimization, if both the source and destination are byte aligned can do a straight memcpy.
			//can safely >>3 to get byte index as we know we are byte aligned. no need for bits_to_bytes which rounds up.
			::memcpy_s(
				bits, (bit_count >> 3),
				&_data[_data_bit_pos >> 3], (bit_count >> 3));
			_data_bit_pos += bit_count;
		}
		else {
			::memset(bits, 0, bits_to_bytes(bit_count));

			unsigned int byte_to_read_index = 0;
			unsigned int remaining_bit_count_to_read = bit_count;
			while (remaining_bit_count_to_read > 0) {				
				unsigned char* byte_to_read = &bits[byte_to_read_index];
				byte_to_read_index++;

				if (data_bit_offset == 0) {
					//destination is byte aligned, can do straight copy
					*byte_to_read = _data[_data_bit_pos >> 3];
				}
				else {
					*byte_to_read |= (_data[_data_bit_pos >> 3] >> data_bit_offset);

					if (remaining_bit_count_to_read > (8 - data_bit_offset))
					{
						*byte_to_read |= _data[(_data_bit_pos >> 3) + 1] << (8 - data_bit_offset);
					}
				}

				if (remaining_bit_count_to_read < 8) {
					//mask out the top bits because they belong to the next chunk of bits
					//ex. if only 3 bits were remaining to read then the top 5 bits don't belong to this byte
					int mask_length = 8 - remaining_bit_count_to_read;
					unsigned char mask = 0xFF >> mask_length;
					*byte_to_read &= mask;
				}

				if (remaining_bit_count_to_read >= 8) {
					_data_bit_pos += 8;
					remaining_bit_count_to_read -= 8;
				}
				else {
					_data_bit_pos += remaining_bit_count_to_read;
					remaining_bit_count_to_read = 0;
				}
			}
		}

		return true;
	}

}