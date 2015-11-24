#pragma once

#include <string>
#include "win32_headers.h"
#include "win32_cli_app_error_behavoir.h"
#include "solar/utility/property.h"

namespace solar{

	struct win32_cli_app_setup_params {
		MAKE_PROPERTY(win32_cli_app_setup_params, win32_cli_app_error_behavoir, assert_behavoir, win32_cli_app_error_behavoir::THROW);
		MAKE_PROPERTY(win32_cli_app_setup_params, win32_cli_app_error_behavoir, alert_behavoir, win32_cli_app_error_behavoir::THROW);
	};

	class win32_cli_app {
	private:
		win32_cli_app_setup_params _setup_params;
		HANDLE _error_handle;
		HANDLE _output_handle;
		int _error_count;

	public:
		win32_cli_app();

		void setup(const win32_cli_app_setup_params& params);
		void write_to_error_console(const std::string& message);
		int get_error_count() const;

	private:
		bool handle_assert_failed(const char* file_name, int line_number, const char* expression);
		bool handle_alert(const char* file_name, int line_number, const char* message);
		void handle_trace(const char* file_name, int line_number, bool add_newline, const char* message);
	};

}