#pragma once

#include <string>

namespace solar {

	class file_dialog_filter {
	public:
		std::string _description;
		std::string _pattern;

	public:
		file_dialog_filter(const char* description, const char* pattern)
			: _description(description)
			, _pattern(pattern) {
		}
	};


}