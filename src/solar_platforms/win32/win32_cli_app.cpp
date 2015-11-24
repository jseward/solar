#include "win32_cli_app.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_build.h"
#include "solar/strings/string_helpers.h"

namespace solar {

	win32_cli_app::win32_cli_app() 
		: _error_handle(nullptr)
		, _output_handle(nullptr) 
		, _error_count(0) {
	}

	void win32_cli_app::setup(const win32_cli_app_setup_params& params) {

		_setup_params = params;

		_error_handle = ::GetStdHandle(STD_ERROR_HANDLE);
		_output_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

		set_assert_failed_handler([&](const char* file_name, int line_number, const char* expression) {
			return handle_assert_failed(file_name, line_number, expression); 
		});

		set_alert_handler([&](const char* file_name, int line_number, const char* message) {
			return handle_alert(file_name, line_number, message);
		});

		set_trace_handler([&](const char* file_name, int line_number, bool add_newline, const char* message) {
			handle_trace(file_name, line_number, add_newline, message);
		});
	}
	
	int win32_cli_app::get_error_count() const {
		return _error_count;
	}

	bool win32_cli_app::handle_assert_failed(const char* file_name, int line_number, const char* expression) {
		_error_count++;

		auto formatted_message = build_string("{}:{} ASSERT({})\n", file_name, line_number, expression);
		print_to_debug_output(formatted_message);
		
		switch (_setup_params.get_assert_behavoir()) {
		case win32_cli_app_error_behavoir::THROW:
			throw std::runtime_error(formatted_message);
		case win32_cli_app_error_behavoir::WRITE_AND_BREAK:
			write_to_error_console(formatted_message);
			return true;
		case win32_cli_app_error_behavoir::WRITE_AND_CONTINUE:
			write_to_error_console(formatted_message);
			return false;
		}
		return false;
	}

	bool win32_cli_app::handle_alert(const char* file_name, int line_number, const char* message) {
		UNUSED_PARAMETER(file_name);
		UNUSED_PARAMETER(line_number);

		_error_count++;

		auto formatted_message = build_string("{}\n", message);
		print_to_debug_output(formatted_message);

		switch (_setup_params.get_alert_behavoir()) {
		case win32_cli_app_error_behavoir::THROW:
			throw std::runtime_error(formatted_message);
		case win32_cli_app_error_behavoir::WRITE_AND_BREAK:
			write_to_error_console(formatted_message);
			return true;
		case win32_cli_app_error_behavoir::WRITE_AND_CONTINUE:
			write_to_error_console(formatted_message);
			return false;
		}
		return false;
	}

	void win32_cli_app::handle_trace(const char* file_name, int line_number, bool add_newline, const char* message) {
		UNUSED_PARAMETER(file_name);
		UNUSED_PARAMETER(line_number);
		::WriteConsoleA(_output_handle, message, get_string_length(message), nullptr, nullptr);
		::WriteConsoleA(_output_handle, "\n", 1, nullptr, nullptr);
		print_to_debug_output(message);
		if (add_newline) {
			print_to_debug_output("\n");
		}
	}

	void win32_cli_app::write_to_error_console(const std::string& message) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		::GetConsoleScreenBufferInfo(_error_handle, &info);
		::SetConsoleTextAttribute(_error_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		::WriteConsoleA(_error_handle, message.c_str(), message.length(), nullptr, nullptr);
		::SetConsoleTextAttribute(_error_handle, info.wAttributes);
	}

}