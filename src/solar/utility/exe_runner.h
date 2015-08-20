#pragma once

#include <string>

namespace solar {

	class exe_runner {
	public:
		struct run_exe_result {
			bool _success;
			std::string _error_message;
			int _exit_code;
			std::string _std_out;
			std::string _std_err;

			run_exe_result()
				: _success(false)
				, _exit_code(-1)
			{}
		};

		run_exe_result make_error_result(const std::string& message) {
			run_exe_result result;
			result._success = false;
			result._error_message = message;
			return result;
		}

		virtual run_exe_result run_exe(const std::string& exe_path, const std::string& arguments) = 0;
	};

}