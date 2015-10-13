#pragma once

#include "stream.h"

namespace solar {

	class memory_stream : public stream {
	private:
		char* _writable_memory;
		const char* _readable_memory;
		unsigned int _memory_size;
		unsigned int _current_pos;

	public:
		virtual ~memory_stream();

		virtual bool can_have_BOM() const override;
		virtual void seek(stream_seek_anchor anchor, long offset) override;
		virtual unsigned int read_bytes(char* buffer, unsigned int max_count) override;
		virtual void write_bytes(const char* bytes, unsigned int count) override;
		virtual void flush() override;
		virtual std::string get_description() const override;

	private:
		memory_stream();
		friend memory_stream make_writable_memory_stream(char* memory, unsigned int memory_size);
		friend memory_stream make_readable_memory_stream(const char* memory, unsigned int memory_size);
	};

}