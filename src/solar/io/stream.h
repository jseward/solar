#pragma once

#include <string>
#include "stream_seek_anchor.h"

namespace solar {

	class stream {
	public:
		virtual bool can_have_BOM() const = 0;
		virtual void seek(stream_seek_anchor anchor, long offset) = 0;
		virtual unsigned int read_bytes(char* buffer, unsigned int max_count) = 0;
		virtual void write_bytes(const char* bytes, unsigned int count) = 0;
		virtual void flush() = 0;
		virtual std::string get_description() const = 0;
	};

}