#pragma once

#include "archive_writer.h"

namespace solar {

	//writes directly to memory using only enough bits as required. (ex. bool is one bit)

	class bitstream_archive_writer : public archive_writer {
	private:
		unsigned char* _preallocated_data;
		size_t _preallocated_data_size;

		unsigned char* _data;
		size_t _data_bit_pos;
		size_t _data_size_in_bits;

	public:
		bitstream_archive_writer(unsigned char* preallocated_data, size_t preallocated_data_size);
		virtual ~bitstream_archive_writer();

		const unsigned char* get_data() const;
		size_t get_data_size() const;

		virtual void begin_writing() override;
		virtual void end_writing() override;

		virtual void write_name(const char* name) override;
		virtual void write_array(unsigned int size, std::function<void(archive_writer&, unsigned int)> write_element_func) override;
		virtual void write_object(std::function<void(archive_writer&)> write_object_func) override;
		virtual void write_bool(bool value) override;
		virtual void write_uint16(uint16_t value) override;
		virtual void write_int(int value, const archive_int_compression& compression) override;
		virtual void write_int64(int64_t value) override;
		virtual void write_uint(unsigned int value) override;
		virtual void write_float(float value) override;
		virtual void write_string(const std::string& value) override;
		virtual void write_color(const color& value) override;

	private:
		void make_room_for_bits(unsigned int bit_count);
		void write_bit(unsigned char bit);
		void write_bits(const unsigned char* bits, unsigned int bit_count);

		template<typename T> void write_atomic_value(const T& value) {
			write_bits(reinterpret_cast<const unsigned char*>(&value), sizeof(T) * 8);
		}
	};

}