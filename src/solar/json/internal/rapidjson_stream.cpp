#include "rapidjson_stream.h"

#include "solar/utility/assert.h"

namespace solar {

	rapidjson_stream::rapidjson_stream(stream& stream)
		: _stream(stream)
		, _current(0)
		, _count(0) {

		Read();
	}

	char rapidjson_stream::Peek() {
		return _current;
	}

	char rapidjson_stream::Take() {
		char c = _current;
		Read();
		return c;
	}

	size_t rapidjson_stream::Tell() const {
		return _count;
	}

	void rapidjson_stream::Put(char c) {
		_stream.write_bytes(&c, 1);
	}

	char* rapidjson_stream::PutBegin() {
		ASSERT(false);
		return nullptr;
	}

	size_t rapidjson_stream::PutEnd(char*) {
		ASSERT(false);
		return 0;
	}

	void rapidjson_stream::Read() {
		int r = _stream.read_bytes(&_current, 1);
		if (r == 1) {
			_count++;
		}
		else {
			_current = '\0';
		}
	}

	void rapidjson_stream::Flush() {
		_stream.flush();
	}

}
