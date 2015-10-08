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
		virtual void raise_error(const std::string& error_message) override;
		virtual void read_object(const char* name, archivable& value) override;
		virtual void read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) override;
		virtual void read_bool(const char* name, bool& value) override;
		virtual void read_ushort(const char* name, unsigned short& value) override;
		virtual void read_ushorts_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, unsigned short)> handle_value_func) override;
		virtual void read_ushorts_fixed(const char* name, unsigned int size, unsigned short* values_begin) override;
		virtual void read_int(const char* name, int& value, const archive_int_compression& compression) override;
		virtual void read_ints_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, int)> handle_value_func) override;
		virtual void read_ints_fixed(const char* name, unsigned int size, int* values_begin) override;
		virtual void read_optional_int(const char* name, optional<int>& value) override;
		virtual void read_int64(const char* name, int64_t& value) override;
		virtual void read_uint(const char* name, unsigned int& value) override;
		virtual void read_float(const char* name, float& value) override;
		virtual void read_floats_dynamic(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(unsigned int, float)> handle_value_func) override;
		virtual void read_floats_fixed(const char* name, unsigned int size, float* values_begin) override;
		virtual void read_string(const char* name, std::string& value) override;

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