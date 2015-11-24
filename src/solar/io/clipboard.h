#pragma once

#include <string>

namespace solar {

	class clipboard {
	public:
		virtual void set_clipboard_text(const std::wstring& text) = 0;
		virtual std::wstring get_clipboard_text() = 0;
	};

}