#include "win32_error.h"

#include "win32_headers.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_build.h"

namespace solar {

	std::string get_last_win32_error_message() {
		return get_win32_error_message(::GetLastError());
	}

	std::string get_win32_error_message(int error_code) {
		LPVOID format_buffer = NULL;
		::FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			error_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(wchar_t*)&format_buffer,
			0,
			NULL);
		if (format_buffer != NULL) {
			std::string error_message = utf16_to_utf8((wchar_t*)format_buffer);
			::LocalFree(format_buffer);
			return error_message;
		}
		else {
			return build_string("??? ({})", error_code);
		}
	}

}
