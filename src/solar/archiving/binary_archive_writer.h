#pragma once

#include <memory>
#include "solar/io/file_stream.h"
#include "solar/utility/verify.h"
#include "archive_writer.h"

namespace solar {

	class binary_archive_writer : public archive_writer {

	private:
		stream& _stream;

	public:
		binary_archive_writer(stream& stream);
		virtual ~binary_archive_writer();

		virtual void begin_writing() override;
		virtual void end_writing() override;
		virtual void write_object(const char* name, const archivable& value) override;
		virtual void write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) override;
		virtual void write_bool(const char* name, bool value) override;
		virtual void write_ushort(const char* name, unsigned short value) override;
		virtual void write_ushorts_dynamic(const char* name, unsigned int size, std::function<unsigned short(unsigned int)> get_value_at_func) override;
		virtual void write_ushorts_fixed(const char* name, unsigned int size, const unsigned short* values_begin) override;
		virtual void write_int(const char* name, int value, const archive_int_compression& compression) override;
		virtual void write_ints_dynamic(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) override;
		virtual void write_ints_fixed(const char* name, unsigned int size, const int* values_begin) override;
		virtual void write_optional_int(const char* name, const optional<int>& value) override;
		virtual void write_int64(const char* name, int64_t value) override;
		virtual void write_uint(const char* name, unsigned int value) override;
		virtual void write_float(const char* name, float value) override;
		virtual void write_floats_dynamic(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) override;
		virtual void write_floats_fixed(const char* name, unsigned int size, const float* values_begin) override;
		virtual void write_string(const char* name, const std::string& value) override;

	private:
		template<typename T> void write_atomic_value(const T& value);
	};

	template<typename T> inline void binary_archive_writer::write_atomic_value(const T& value) {
		_stream.write_bytes(reinterpret_cast<const char*>(&value), sizeof(T));
	}

}