#pragma once

#include <string>

namespace solar {

	class file_change_handler {
	public:
		virtual void on_file_changed(const std::string& path, void* data) = 0;
	};

}