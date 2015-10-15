#pragma once

#include <memory>
#include "solar/io/file_stream.h"
#include "solar/utility/verify.h"
#include "archive_reader.h"

namespace solar {

	class binary_archive_reader : public archive_reader {

	private:
		stream& _stream;

	public:
		binary_archive_reader(stream& stream);
		virtual ~binary_archive_reader();

		virtual std::string get_source_description() const override;
		virtual void raise_error(const std::string& error_message) override;
		virtual unsigned int get_read_position() const override;
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
		template<typename T> void read_atomic_value(T& value);
	};

	template<typename T> inline void binary_archive_reader::read_atomic_value(T& value) {
		VERIFY(_stream.read_bytes(reinterpret_cast<char*>(&value), sizeof(T)) == sizeof(T));
	}

}