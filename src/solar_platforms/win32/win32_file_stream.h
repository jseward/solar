#pragma once

#include "solar/io/file_stream.h"

namespace solar {

	class win32_file_stream : public file_stream {
	private:
		std::string _path;
		FILE* _handle;
	
	public:
		win32_file_stream(const std::string& path, FILE* handle);
		virtual ~win32_file_stream();
		virtual const char* get_file_path() const override;
		virtual unsigned int get_file_size() const override;
		virtual bool can_have_BOM() const override;
		virtual void seek(stream_seek_anchor anchor, long offset) override;
		virtual unsigned int read_bytes(char* buffer, unsigned int max_count) override;
		virtual void write_bytes(const char* bytes, unsigned int count) override;
		virtual void flush() override;
		virtual std::string get_description() const override;
		FILE* get_handle();
	};

}