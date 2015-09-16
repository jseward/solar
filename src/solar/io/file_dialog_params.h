#pragma once

#include <vector>
#include "file_dialog_filter.h"

namespace solar {

	class file_dialog_params {
	public:
		std::vector<file_dialog_filter> _filters;
		std::string _initial_directory;
		std::string _auto_append_extension;

	public:
		file_dialog_params& add_filter(const file_dialog_filter& filter) {
			_filters.push_back(filter);
			return *this;
		}

		file_dialog_params& add_filter(const char* description, const char* pattern) {
			_filters.emplace_back(description, pattern);
			return *this;
		}

		file_dialog_params& set_initial_directory(const std::string& initial_directory) {
			_initial_directory = initial_directory;
			return *this;
		}

		file_dialog_params& set_auto_append_extension(const std::string& extension) {
			_auto_append_extension = extension;
			return *this;
		}
	};


}