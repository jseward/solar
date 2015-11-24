#pragma once

#include <memory>
#include "solar/io/stream.h"
#include "solar/json/json_writer.h"
#include "archive_writer.h"

namespace solar {

	class json_archive_writer : public archive_writer {

	private:
		json_writer _writer;
		bool _is_writing;

	public:
		explicit json_archive_writer(stream& stream);
		virtual ~json_archive_writer();

		virtual void begin_writing() override;
		virtual void end_writing() override;
		virtual void write_object(const char* name, const archivable& value) override;
		virtual void write_objects(const char* name, unsigned int size, std::function<void(archive_writer&, unsigned int)> write_object_func) override;
		virtual void write_bool(const char* name, bool value) override;
		virtual void write_uint16(const char* name, uint16_t value) override;
		virtual void write_uint16s_dynamic(const char* name, unsigned int size, std::function<uint16_t(unsigned int)> get_value_at_func) override;
		virtual void write_uint16s_fixed(const char* name, unsigned int size, const uint16_t* values_begin) override;
		virtual void write_int(const char* name, int value, const archive_int_compression& compression) override;
		virtual void write_ints_dynamic(const char* name, unsigned int size, std::function<int(unsigned int)> get_value_at_func) override;
		virtual void write_ints_fixed(const char* name, unsigned int size, const int* values_begin) override;
		virtual void write_optional_int(const char* name, const optional<int>& value) override;
		virtual void write_int64(const char* name, int64_t value);
		virtual void write_uint(const char* name, unsigned int value) override;
		virtual void write_float(const char* name, float value) override;
		virtual void write_floats_dynamic(const char* name, unsigned int size, std::function<float(unsigned int)> get_value_at_func) override;
		virtual void write_floats_fixed(const char* name, unsigned int size, const float* values_begin) override;
		virtual void write_string(const char* name, const std::string& value) override;
		virtual void write_color(const char* name, const color& value) override;
	};

}