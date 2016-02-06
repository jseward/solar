#pragma once

#include "archive_reader.h"

namespace solar {

	class bitstream_archive_reader : public archive_reader {
	private:
		const unsigned char* _data;
		size_t _data_size;
		size_t _data_size_in_bits;

		size_t _data_bit_pos;

	public:
		bitstream_archive_reader(const unsigned char* data, size_t data_size);
		virtual ~bitstream_archive_reader();

		virtual std::string get_source_description() const override;
		virtual void raise_error(const std::string& error_message) const override;
		virtual unsigned int get_read_position() const override;
		virtual void read_name(const char* name) override;
		virtual void read_array(
			std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
			std::function<void(archive_reader&, unsigned int)> read_element_func) override;
		virtual void read_object(std::function<void(archive_reader&)> read_object_func) override;
		virtual void read_bool(bool& value) override;
		virtual void read_uint16(uint16_t& value) override;
		virtual void read_int(int& value, const archive_int_compression& compression) override;
		virtual void read_int64(int64_t& value) override;
		virtual void read_uint(unsigned int& value) override;
		virtual void read_float(float& value) override;
		virtual void read_string(std::string& value) override;
		virtual void read_color(color& value) override;

	private:
		bool read_bit(unsigned char* bit);
		bool read_bits(unsigned char* bits, unsigned int bit_count);

		template<typename T> T read_atomic_value() {
			T value;
			read_bits(reinterpret_cast<unsigned char*>(&value), sizeof(T) * 8);
			return value;
		}
	};

}