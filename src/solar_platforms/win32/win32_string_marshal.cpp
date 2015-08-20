#include "solar/utility/alert.h"
#include "solar/utility/verify.h"
#include "win32_headers.h"
#include "win32_error.h"

namespace solar {

	void utf8_to_utf16(std::wstring& out, const char* utf8) {
		out = L"";

		int utf8_length = strlen(utf8);
		if (utf8_length != 0) { //MultiByteToWideChar doesn't handle 0 length strings as 0 is considered a failed call

			int utf16_length = ::MultiByteToWideChar(CP_UTF8, 0, utf8, utf8_length, 0, 0);
			if (utf16_length == 0) {
				ALERT("MultiByteToWideChar failed.\nutf8: '{}'\nerror: {}", utf8, get_last_win32_error_message());
				out = L"???";
			}
			else {
				out.resize(utf16_length, L'\0');

				int result = ::MultiByteToWideChar(CP_UTF8, 0, utf8, utf8_length, &out[0], utf16_length);
				if (result == 0) {
					ALERT("MultiByteToWideChar failed.\nutf8: '{}'\nerror: {}", utf8, get_last_win32_error_message());
					out = L"???";
				}
			}
		}
	}

	void utf16_to_utf8(std::string& out, const wchar_t* utf16) {
		out = "";

		int utf16_length = wcslen(utf16);
		if (utf16_length != 0) { //WideCharToMultiByte doesn't handle 0 length strings as 0 is considered a failed call

			int utf8_length = ::WideCharToMultiByte(CP_UTF8, 0, utf16, utf16_length, NULL, 0, NULL, NULL);
			if (utf8_length == 0) {
				ALERT("WideCharToMultiByte failed.\nerror: {}", get_last_win32_error_message());
				out = "???";
			}
			else {
				out.resize(utf8_length, '\0');

				int result = ::WideCharToMultiByte(CP_UTF8, 0, utf16, utf16_length, &out[0], utf8_length, NULL, NULL);
				if (result == 0) {
					ALERT("WideCharToMultiByte failed.\nerror: {}", get_last_win32_error_message()); //fmt can't handle wchar_t so can't print it
					out = "???";
				}
			}
		}
	}

}