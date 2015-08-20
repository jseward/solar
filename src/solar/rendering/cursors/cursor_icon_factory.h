#pragma once

#include <string>

namespace solar {

	class cursor_icon;

	class cursor_icon_factory {
	public:
		virtual cursor_icon* get_cursor_icon(const std::string& id, const std::string& id_source_description) = 0;
		virtual int get_cursor_icon_caching_context() const = 0;
	};

}