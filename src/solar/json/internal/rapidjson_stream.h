#pragma once

#include <memory>
#include "solar/io/stream.h"

namespace solar {

	//a rapidjson rapidjson_stream that works with solar::stream instead of a raw FILE*
	class rapidjson_stream {
	public:
		typedef char Ch;

	private:
		stream& _stream;
		char _current;
		size_t _count;

	public:
		rapidjson_stream(stream& stream);

		char Peek();
		char Take();
		size_t Tell() const;
		void Put(char c);
		char* PutBegin();
		size_t PutEnd(char*);
		void Flush();

	private:
		void Read();
	};


}