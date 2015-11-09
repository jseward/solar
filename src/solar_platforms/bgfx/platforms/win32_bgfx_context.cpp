#include "win32_bgfx_context.h"

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"
#include "solar/math/rect.h"

namespace solar {

	win32_bgfx_context::win32_bgfx_context() 
		: _hwnd(nullptr)
		, _is_window_active(false)
		, _is_window_minimized(false)
		, _is_window_in_size_move(false) 
		, _is_window_in_menu_loop(false) {
	}

	win32_bgfx_context::~win32_bgfx_context() {
	}

	bool win32_bgfx_context::setup(HWND hwnd, const bgfx_user_settings& settings) {
		_hwnd = hwnd;
		_is_window_active = (::GetForegroundWindow() == hwnd);

		bgfx::winSetHwnd(hwnd);

		if (!bgfx::init(settings.get_RendererType(), 0U, 0U, &_bgfx_callback, nullptr)) {
			ALERT("bgfx::init() failed.");
			return false;
		}

		return apply_user_settings(settings);
	}
	
	void win32_bgfx_context::teardown() {
		bgfx::shutdown();
	}

	void win32_bgfx_context::add_backbuffer_size_changed_callback(backbuffer_size_changed_callback callback) {
		_backbuffer_size_changed_callbacks.push_back(callback);
	}

	bool win32_bgfx_context::apply_user_settings(const bgfx_user_settings& new_settings) {
		ASSERT(_hwnd != nullptr);

		TRACE("win32_bgfx_context apply user settings...");
		new_settings.trace_dump();

		bgfx_user_settings old_settings = _user_settings;
		_user_settings = new_settings;

		DWORD new_style = 0;
		size client_size;

		if (new_settings.get_window_type() == bgfx_window_type::RESIZABLE_WINDOW) {
			new_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
			client_size = new_settings.get_window_client_size();
		}
		else if (new_settings.get_window_type() == bgfx_window_type::VIRTUAL_FULLSCREEN) {
			new_style = WS_OVERLAPPED | WS_SYSMENU;
			client_size = get_desktop_size();
		}
		else {
			ASSERT(false); //unknown window type
		}
		
		::ShowWindow(_hwnd, SW_HIDE);
		::SetWindowLongPtr(_hwnd, GWL_STYLE, new_style);
		::ShowWindow(_hwnd, SW_RESTORE);

		RECT new_window_size = { 0 };
		new_window_size.right = client_size._width;
		new_window_size.bottom = client_size._height;
		VERIFY(::AdjustWindowRect(&new_window_size, GetWindowLong(_hwnd, GWL_STYLE), FALSE) == TRUE);

		int non_client_width = (new_window_size.right - new_window_size.left) - client_size._width;
		int non_client_height = (new_window_size.bottom - new_window_size.top) - client_size._height;

		point new_window_top_left = point(0, 0);
		if (new_settings.get_window_type() == bgfx_window_type::RESIZABLE_WINDOW) {
			new_window_top_left = new_settings.get_window_top_left();
		}

		rect new_window_rect = rect(
			new_window_top_left,
			size(new_window_size.right - new_window_size.left, new_window_size.bottom - new_window_size.top));
		if (new_settings.get_is_window_constrainted_to_desktop()) {
			new_window_rect = make_rect_constrained_within(rect(point(0, 0), get_desktop_size()), new_window_rect);
		}

		::MoveWindow(
			_hwnd,
			new_window_rect.get_left(),
			new_window_rect.get_top(),
			new_window_rect.get_width(),
			new_window_rect.get_height(),
			FALSE);
		TRACE("moved window to {}", new_window_rect);

		reset_bgfx(new_window_rect.get_size() - size(non_client_width, non_client_height));

		return true;
	}

	void win32_bgfx_context::attempt_render_scene(render_scene_functor& func) {
		bool is_paused =
			_is_window_minimized ||
			_is_window_in_size_move ||
			_is_window_in_menu_loop;
		if (!is_paused) {
			func.render_scene();
		}
	}

	void win32_bgfx_context::reset_bgfx(const size& new_backbuffer_size) {
		TRACE("backbuffer_size changed to {}", new_backbuffer_size);
		bgfx::reset(new_backbuffer_size._width, new_backbuffer_size._height, BGFX_RESET_VSYNC);
		_backbuffer_size = new_backbuffer_size;
		for (auto& callbacks : _backbuffer_size_changed_callbacks) {
			callbacks(new_backbuffer_size);
		}
	}

	bool win32_bgfx_context::handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);

		bool handled = false;

		//gracefully handle not attached to any window yet so clients don't have to deal with messages getting
		//sent during to window creation.
		if (_hwnd != nullptr) {
			ASSERT(_hwnd == hwnd);

			switch (message) {
				case WM_SIZE: {
					_is_window_minimized = (wparam == SIZE_MINIMIZED);
					check_if_window_size_changed();
					handled = true;
					break;
				}

				case WM_ENTERSIZEMOVE: {
					_is_window_in_size_move = true;
					handled = true;
					break;
				}

				case WM_EXITSIZEMOVE: {
					_is_window_in_size_move = false;
					check_if_window_size_changed();
					handled = true;
					break;
				}

				case WM_ENTERMENULOOP: {
					_is_window_in_menu_loop = true;
					handled = true;
					break;
				}

				case WM_EXITMENULOOP: {
					_is_window_in_menu_loop = false;
					handled = true;
					break;
				}

				case WM_ACTIVATEAPP: {
					_is_window_active = (wparam == TRUE);
					handled = true;
					break;
				}
			}
		}

		return handled;
	}

	void win32_bgfx_context::check_if_window_size_changed() {
		bool should_check = 
			_user_settings.get_window_type() == bgfx_window_type::RESIZABLE_WINDOW &&
			!_is_window_minimized &&
			!_is_window_in_size_move;

		if (should_check) {
			if (get_window_client_size() != _backbuffer_size) {
				reset_bgfx(get_window_client_size());
			}
		}
	}

	void win32_bgfx_context::toggle_virtual_fullscreen() {

		if (_user_settings.get_window_type() == bgfx_window_type::VIRTUAL_FULLSCREEN) {
			auto new_settings = _user_settings;
			new_settings.set_window_type(bgfx_window_type::RESIZABLE_WINDOW);
			apply_user_settings(new_settings);
		}
		else if (_user_settings.get_window_type() == bgfx_window_type::RESIZABLE_WINDOW) {
			auto new_settings = _user_settings;
			new_settings.set_window_type(bgfx_window_type::VIRTUAL_FULLSCREEN);
			new_settings.set_window_client_size(_backbuffer_size);
			RECT window_rect = { 0 };
			::GetWindowRect(_hwnd, &window_rect);
			new_settings.set_window_top_left(point(window_rect.left, window_rect.top));
			apply_user_settings(new_settings);
		}
		else {
			ASSERT(false);
		}

	}

	size win32_bgfx_context::get_backbuffer_size() const {
		return _backbuffer_size;
	}

	size win32_bgfx_context::get_window_client_size() const {
		RECT client_rect;
		::GetClientRect(_hwnd, &client_rect);
		return size(
			client_rect.right - client_rect.left,
			client_rect.bottom - client_rect.top);
	}

	size win32_bgfx_context::get_desktop_size() const {
		return size(
			::GetSystemMetrics(SM_CXSCREEN),
			::GetSystemMetrics(SM_CYSCREEN));
	}

}