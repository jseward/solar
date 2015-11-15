#include "memory_stream.h"

#include <algorithm>
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	memory_stream::memory_stream() 
		: _readable_memory(nullptr)
		, _writable_memory(nullptr)
		, _memory_size(0)
		, _current_pos(0) {
	}

	memory_stream make_writable_memory_stream(char* memory, unsigned int memory_size) {
		memory_stream ms;
		ms._writable_memory = memory;
		ms._memory_size = memory_size;
		return ms;
	}

	memory_stream make_readable_memory_stream(const char* memory, unsigned int memory_size) {
		memory_stream ms;
		ms._readable_memory = memory;
		ms._memory_size = memory_size;
		return ms;
	}

	memory_stream::~memory_stream() {
	}

	bool memory_stream::can_have_BOM() const {
		return false;
	}

	unsigned int memory_stream::get_size() const {
		return _memory_size;
	}

	void memory_stream::seek(stream_seek_anchor anchor, long offset) {
		UNUSED_PARAMETER(anchor);
		UNUSED_PARAMETER(offset);
		ASSERT(false);//todo
	}

	unsigned int memory_stream::read_bytes(char* buffer, unsigned int max_count) {
		ASSERT(_readable_memory != nullptr);
		unsigned int read_count = std::min(max_count, _memory_size - _current_pos);
		memcpy_s(buffer, max_count, &_readable_memory[_current_pos], read_count);
		_current_pos += read_count;
		return read_count;
	}

	void memory_stream::write_bytes(const char* bytes, unsigned int count) {
		ASSERT(_writable_memory != nullptr);
		IF_VERIFY(count <= (_memory_size - _current_pos)) {
			memcpy_s(&_writable_memory[_current_pos], count, bytes, count);
			_current_pos += count;
		}
	}

	void memory_stream::flush() {
		//do nothing
	}

	std::string memory_stream::get_description() const {
		return "memory_stream";
	}

}