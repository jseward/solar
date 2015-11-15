#include "win32_file_stream.h"

#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/strings/string_build.h"

namespace solar {

	win32_file_stream::win32_file_stream(const std::string& path, FILE* handle) 
		: _path(path)
		, _handle(handle) {
	}

	win32_file_stream::~win32_file_stream() {
	}

	const char* win32_file_stream::get_file_path() const {
		return _path.c_str();
	}

	unsigned int win32_file_stream::get_size() const {
		ASSERT(::ftell(_handle) == 0);
		VERIFY(::fseek(_handle, 0, SEEK_END) == 0);
		unsigned int size = ::ftell(_handle);
		VERIFY(::fseek(_handle, 0, SEEK_SET) == 0);
		return size;
	}

	bool win32_file_stream::can_have_BOM() const {
		return true;
	}

	void win32_file_stream::seek(stream_seek_anchor anchor, long offset) {
		int origin = 0;
		if (anchor == stream_seek_anchor::BEGIN) {
			origin = SEEK_SET;
		}
		else if (anchor == stream_seek_anchor::CURRENT) {
			origin = SEEK_CUR;
		}
		else if (anchor == stream_seek_anchor::END) {
			origin = SEEK_END;
		}
		else {
			ASSERT(false);
		}

		int result = ::fseek(_handle, offset, origin);
		if (result != 0) {
			ALERT("File seek failed.\npath : {}", _path);
		}
	}

	unsigned int win32_file_stream::read_bytes(char* buffer, unsigned int max_count) {
		return ::fread(buffer, sizeof(char), max_count, _handle);
	}

	void win32_file_stream::write_bytes(const char* bytes, unsigned int count) {
		size_t result = ::fwrite(bytes, sizeof(char), count, _handle);
		if (result != count) {
			ALERT("File write_bytes failed.\npath : {}\ncount : {}", _path, count);
		}
	}

	void win32_file_stream::flush() {
		::fflush(_handle);
	}

	std::string win32_file_stream::get_description() const {
		return build_string("{{ path:{} }}", _path);
	}

	FILE* win32_file_stream::get_handle() {
		return _handle;
	}

}
