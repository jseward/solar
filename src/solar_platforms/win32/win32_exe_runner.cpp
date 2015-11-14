#include "win32_exe_runner.h"
#include <memory>
#include <functional>
#include "win32_headers.h"
#include "win32_error.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_build.h"
#include "solar/utility/trace.h"
#include "solar/io/file_path_helpers.h"

namespace solar {

	namespace win32_exe_runner_impl {

		//makes handling of cleaning up std_out,std_err pipes nicer.
		class std_pipe {
		private:
			HANDLE _read;
			HANDLE _write;

		public:
			std_pipe()
				: _read(nullptr)
				, _write(nullptr) {
			}

			~std_pipe() {
				close_read();
				close_write();
			}

			HANDLE get_write() {
				return _write;
			}

			void close_read() {
				if (_read != nullptr) {
					::CloseHandle(_read);
					_read = nullptr;
				}
			}

			void close_write() {
				if (_write != nullptr) {
					::CloseHandle(_write);
					_write = nullptr;
				}
			}

			bool create(SECURITY_ATTRIBUTES sa) {
				if (!CreatePipe(&_read, &_write, &sa, 0)) {
					return false;
				}

				if (!SetHandleInformation(_read, HANDLE_FLAG_INHERIT, 0)) {
					return false;
				}

				return true;
			}

			std::string read() {
				close_write();

				size_t buffer_size = 2048;
				auto buffer = std::unique_ptr<char[]>(new char[buffer_size]);

				std::string s;
				for (;;) {
					DWORD read_count = 0;
					BOOL read_success = ::ReadFile(_read, buffer.get(), buffer_size, &read_count, 0);
					if (!read_success || read_count == 0) {
						break;
					}
					//std::min in case full buffer_size has been read in!
					buffer.get()[std::min(buffer_size - 1, static_cast<size_t>(read_count))] = '\0';
					s.append(buffer.get());
				}
				return s;
			}
		};

	}

	exe_runner::run_exe_result win32_exe_runner::run_exe(const std::string& exe_path, const std::string& arguments) {

		//http://stackoverflow.com/questions/7018228/how-do-i-redirect-output-to-a-file-with-createprocess
		//http://msdn.microsoft.com/en-ca/library/windows/desktop/ms682499(v=vs.85).aspx
		//http://stackoverflow.com/questions/7037257/reading-from-stdout-of-a-process-created-with-createprocesswithlogonw

		SECURITY_ATTRIBUTES sa;
		ZeroMemory(&sa, sizeof(sa));
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		win32_exe_runner_impl::std_pipe std_out;
		if (!std_out.create(sa)) {
			return make_error_result(build_string("failed to create pipe : {}", get_last_win32_error_message()));
		}

		win32_exe_runner_impl::std_pipe std_err;
		if (!std_err.create(sa)) {
			return make_error_result(build_string("failed to create pipe : {}", get_last_win32_error_message()));
		}

		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdInput = NULL;
		si.hStdError = std_err.get_write();
		si.hStdOutput = std_out.get_write();

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		std::string fixed_arguments = get_file_name_no_path_no_extension(exe_path) + " " + arguments;
		auto command_line = utf8_to_utf16(fixed_arguments);
		auto command_line_buffer = std::unique_ptr<wchar_t[]>(new wchar_t[command_line.length() + 1]);
		::StrCpyW(command_line_buffer.get(), command_line.c_str());

		if (!::CreateProcessW(
			utf8_to_utf16(exe_path).c_str(),
			command_line_buffer.get(),
			NULL,
			NULL,
			TRUE,
			0,
			NULL,
			NULL,
			&si,
			&pi)) {

			return make_error_result(build_string("CreateProcess failed : {}", get_last_win32_error_message()));
		}
		
		::WaitForSingleObject(pi.hProcess, INFINITE);

		run_exe_result result;

		DWORD exit_code = 0;
		if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
			result._success = true;
			result._exit_code = exit_code;
		}
		else {
			result = make_error_result(build_string("GetExitCodeProcess failed : {}", get_last_win32_error_message()));
		}
				 
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);

		result._std_out = std_out.read();
		result._std_err = std_err.read();

		return result;
	}

}
