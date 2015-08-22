#include "win32_windowed_app.h"

#include "win32_error.h"
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_marshal.h"
#include "solar/strings/string_convert.h"
#include "solar/strings/string_build.h"
#include "solar/strings/string_helpers.h"
#include "solar/time/real_time.h"
#include "solar/time/time_span.h"
#include "solar/time/date_time.h"

namespace solar {

	void win32_windowed_app_setup_params::trace_dump() const {
		TRACE("hinstance : {:x}", (unsigned int)get_hinstance());
		TRACE("window_class_name : {}", utf16_to_utf8(get_window_class_name()));
		TRACE("window_caption : {}", utf16_to_utf8(get_window_caption()));
		TRACE("min_window_size : {}", get_min_window_size());
		TRACE("small_icon_id : {}", get_small_icon_id());
		TRACE("large_icon_id : {}", get_large_icon_id());
		TRACE("is_visible_by_default : {}", bool_to_string(get_is_visible_by_default()));
	}

	win32_windowed_app* win32_windowed_app::s_wnd_proc_context = nullptr;

	win32_windowed_app::win32_windowed_app(file_system& file_system)
		: _file_system(file_system)
		, _hwnd(nullptr) {
	}

	bool win32_windowed_app::setup(const win32_windowed_app_setup_params& params) {

		set_assert_failed_handler([&](const char* file_name, int line_number, const char* expression) {
			return handle_assert_failed(file_name, line_number, expression);
		});

		set_alert_handler([&](const char* file_name, int line_number, const char* message) {
			return handle_alert(file_name, line_number, message);
		});

		set_trace_handler([&](const char* file_name, int line_number, const char* message) {
			handle_trace(file_name, line_number, message);
		});

		TRACE("win32_windowed_app setup...");
		params.trace_dump();
		
		ASSERT(s_wnd_proc_context == nullptr);
		s_wnd_proc_context = this;

		ASSERT(params.get_hinstance() != nullptr);
		ASSERT(!params.get_window_class_name().empty());
		ASSERT(!params.get_window_caption().empty());
		ASSERT(params.get_idle_proc() != nullptr);
		
		_setup_params = params;

		WNDCLASSEX wc = build_wnd_class_ex(params);
		if (!::RegisterClassEx(&wc)) {
			ALERT("RegisterClassEx failed : {0}", get_last_win32_error_message());
			return false;
		}

		DWORD style = WS_OVERLAPPEDWINDOW;
		if (params.get_is_visible_by_default()) {
			style |= WS_VISIBLE;
		}

		_hwnd = ::CreateWindow(
			params.get_window_class_name().c_str(),
			params.get_window_caption().c_str(),
			style,
			0, 0, 0, 0,
			nullptr,
			nullptr,
			params.get_hinstance(),
			nullptr);

		if (_hwnd == nullptr) {
			ALERT("CreateWindow failed : {0}", get_last_win32_error_message());
			return false;
		}

		_error_dialog.setup(params.get_error_dialog_resources(), _hwnd, params.get_hinstance());

		_process_uid = make_date_time_as_utc_now().to_local_time_string("%m_%d_%H_%M_%S");

		return true;
	}

	WNDCLASSEX win32_windowed_app::build_wnd_class_ex(const win32_windowed_app_setup_params& params) {

		WNDCLASSEX wc = { 0 };

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = wnd_proc;
		wc.hInstance = params.get_hinstance();
		wc.lpszClassName = params.get_window_class_name().c_str();
		wc.hIcon = ::LoadIcon(params.get_hinstance(), MAKEINTRESOURCE(params.get_large_icon_id()));
		wc.hIconSm = ::LoadIcon(params.get_hinstance(), MAKEINTRESOURCE(params.get_small_icon_id()));

		//NOTE: important that hbrBackground is NULL. It used to be black but this caused a brief black flicker when the game
		//transitioned from raw SplashScreen (before engine setup) to LoginPanel SplashScreen (after engine setup). The problem
		//seems to be that the message pump is run for the first time after the engine is setup and gets WM_PAINT and/or WM_ERASEBKGND
		//painting the window black. Setting background to NULL fixes it. This does make the resizing of the window slightly worse
		//but that is a reasonable trade off.
		wc.hbrBackground = NULL;

		return wc;
	}

	int win32_windowed_app::run() {
		MSG msg = { 0 };
		msg.message = WM_NULL;
		while (msg.message != WM_QUIT) {
			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else {
				idle();
			}
		}
		return msg.wParam;
	}

	void win32_windowed_app::idle() {
		_setup_params.get_idle_proc()();
	}

	LRESULT win32_windowed_app::wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

		bool handled = false;

		for (auto handler : s_wnd_proc_context->_setup_params.get_wnd_proc_handlers()) {
			if (handler(hwnd, message, wparam, lparam)) {
				handled = true;
				break;
			}
		}

		if (!handled) {
			switch (message)
			{
				case WM_GETMINMAXINFO: {
					MINMAXINFO* minMaxInfo = (MINMAXINFO*)lparam;
					minMaxInfo->ptMinTrackSize.x = s_wnd_proc_context->_setup_params.get_min_window_size().get_width();
					minMaxInfo->ptMinTrackSize.y = s_wnd_proc_context->_setup_params.get_min_window_size().get_height();
					handled = true;
					break;
				}

				case WM_DESTROY: {
					::PostQuitMessage(0);
					handled = true;
					break;
				}
			}
		}

		LRESULT result = 0;
		if (!handled) {
			result = DefWindowProc(hwnd, message, wparam, lparam);
		}

		return result;
	}

	HWND win32_windowed_app::get_hwnd() const {
		return _hwnd;
	}

	const std::string& win32_windowed_app::get_processs_uid() const {
		return _process_uid;
	}

	bool win32_windowed_app::handle_assert_failed(const char* file_name, int line_number, const char* expression) {
		if (_skip_error_registry.contains(file_name, line_number)) {
			return false;
		}

		print(build_string("ASSERT({2}) @ {0}:{1}\n", file_name, line_number, expression).c_str());

		auto result = _error_dialog.show(
			build_string("ASSERT({})", expression).c_str(),
			build_string("{}:{}\n\nASSERT({})", file_name, line_number, expression).c_str());
		if (result == win32_error_dialog_result::SKIP_ALL) {
			_skip_error_registry.add(file_name, line_number);
		}

		return (result == win32_error_dialog_result::BREAK);
	}

	bool win32_windowed_app::handle_alert(const char* file_name, int line_number, const char* message) {
		if (_skip_error_registry.contains(file_name, line_number)) {
			return false;
		}

		print("-------\n");
		print(build_string("ALERT @ {0}:{1}\n", file_name, line_number).c_str());
		print(message);
		print("\n");
		print("-------\n");

		auto result = _error_dialog.show("ALERT", message);
		if (result == win32_error_dialog_result::SKIP_ALL) {
			_skip_error_registry.add(file_name, line_number);
		}

		return (result == win32_error_dialog_result::BREAK);
	}

	void win32_windowed_app::handle_trace(const char* file_name, int line_number, const char* message) {
		UNUSED_PARAMETER(file_name);
		UNUSED_PARAMETER(line_number);
		print(build_string("[{}] {}\n", time_span().build_from_seconds(get_real_time_in_seconds()).to_hours_minutes_seconds_string(), message).c_str());
	}

	void win32_windowed_app::open_log_file(const std::string& path) {
		_log_file = make_file_stream_ptr(_file_system, path, file_mode::CREATE_WRITE);
		TRACE("opened log file : {}", path);
	}

	void win32_windowed_app::print(const char* string) {
		print_to_debug_output(string);

		if (_log_file != nullptr) {
			_log_file->write_bytes(string, get_string_length(string));
		}
	}

}