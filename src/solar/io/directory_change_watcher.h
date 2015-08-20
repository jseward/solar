#pragma once

#include <string>
#include "directory_change.h"

namespace solar {

	class directory_change_watcher {
	public:
		virtual void begin_watching_directory(const std::string& dir_path, bool is_recursive) = 0;
		virtual void end_watching_directory() = 0;
		virtual bool try_pop_directory_change(directory_change& change) = 0;
	};

}