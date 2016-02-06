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
		void try_write_name_before_value();
	};

}