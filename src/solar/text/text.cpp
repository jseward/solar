#include "text.h"

namespace solar {

	text::text(const std::wstring& string)
		: _string(string) {
	}

	text::operator const wchar_t*() const {
		return _string.c_str();
	}

}