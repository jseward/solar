#pragma once

#include <string>

namespace solar {

	void utf8_to_utf16(std::wstring& utf16_out, const char* utf8);
	void utf16_to_utf8(std::string& utf8_out, const wchar_t* utf16);

	inline std::wstring utf8_to_utf16(const char* utf8) {
		std::wstring out;
		utf8_to_utf16(out, utf8);
		return out;
	}

	inline std::wstring utf8_to_utf16(const std::string& utf8) {
		return utf8_to_utf16(utf8.c_str());
	}

	inline std::string utf16_to_utf8(const wchar_t* utf16) {
		std::string out;
		utf16_to_utf8(out, utf16);
		return out;
	}

	inline std::string utf16_to_utf8(const std::wstring& utf16) {
		return utf16_to_utf8(utf16.c_str());
	}

}