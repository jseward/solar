#pragma once

#include <string>

namespace solar {

	std::string get_last_win32_error_message();
	std::string get_win32_error_message(int error_code);

}