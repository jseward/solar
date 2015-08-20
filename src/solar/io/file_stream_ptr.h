#pragma once

#include <memory>
#include <functional>
#include "file_mode.h"
#include "file_system.h"

namespace solar {

	typedef std::unique_ptr<file_stream, std::function<void(file_stream*)>> file_stream_ptr;

	inline file_stream_ptr make_file_stream_ptr(file_system& file_system, const std::string& path, file_mode file_mode) {
		auto del = [&file_system](file_stream* fs) { file_system.close_file_stream(fs); };
		return file_stream_ptr(file_system.open_file_stream(path, file_mode), del);
	}

}