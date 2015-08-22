#pragma once

#include <vector>
#include <functional>
#include "solar/utility/property.h"
#include "solar/math/size.h"
#include "solar/io/file_stream_ptr.h"
#include "win32_headers.h"
#include "win32_error_dialog.h"
#include "win32_skip_error_registry.h"

namespace solar{

	typedef std::function<bool(HWND, UINT, WPARAM, LPARAM)> win32_windowed_app_wnd_proc_handler;
	typedef std::function<void()> win32_windowed_app_idle_proc;

	struct win32_windowed_app_setup_params {
		MAKE_PROPERTY(win32_windowed_app_setup_params, HINSTANCE, hinstance, nullptr);
		MAKE_PROPERTY(win32_windowed_app_setup_params, std::wstring, window_class_name, L"");
		MAKE_PROPERTY(win32_windowed_app_setup_params, std::wstring, window_caption, L"");
		MAKE_PROPERTY(win32_windowed_app_setup_params, size, min_window_size, size(200, 200));
		MAKE_PROPERTY(win32_windowed_app_setup_params, int, small_icon_id, 0);
		MAKE_PROPERTY(win32_windowed_app_setup_params, int, large_icon_id, 0);
		MAKE_PROPERTY(win32_windowed_app_setup_params, bool, is_visible_by_default, true);
		MAKE_PROPERTY(win32_windowed_app_setup_params, std::vector<win32_windowed_app_wnd_proc_handler>, wnd_proc_handlers, std::vector<win32_windowed_app_wnd_proc_handler>());
		MAKE_PROPERTY(win32_windowed_app_setup_params, win32_windowed_app_idle_proc, idle_proc, nullptr);
		MAKE_PROPERTY(win32_windowed_app_setup_params, win32_error_dialog_resources, error_dialog_resources, win32_error_dialog_resources());

		void trace_dump() const;
	};

	class win32_windowed_app {
		
	private:
		file_system& _file_system;
		win32_windowed_app_setup_params _setup_params;
		win32_error_dialog _error_dialog;
		win32_skip_error_registry _skip_error_registry;
		HWND _hwnd;
		std::string _process_uid;
		file_stream_ptr _log_file;

	public:
		win32_windowed_app(file_system& _file_system);

		bool setup(const win32_windowed_app_setup_params& params);
		int run();
		void open_log_file(const std::string& path);

		HWND get_hwnd() const;
		const std::string& get_processs_uid() const;

	private:
		void idle();
		bool handle_assert_failed(const char* file_name, int line_number, const char* expression);
		bool handle_alert(const char* file_name, int line_number, const char* message);
		void handle_trace(const char* file_name, int line_number, const char* message);
		void print(const char* string);

	private:
		WNDCLASSEX build_wnd_class_ex(const win32_windowed_app_setup_params& params);

	private:
		static win32_windowed_app* s_wnd_proc_context;
		static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	};

}