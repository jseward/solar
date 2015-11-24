#pragma once

#include "stream.h"

namespace solar {

	class file_stream : public stream {
	public:
		virtual const char* get_file_path() const = 0;
	};

}