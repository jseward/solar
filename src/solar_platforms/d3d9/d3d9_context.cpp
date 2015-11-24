#include "d3d9_context.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/utility/trace.h"
#include "solar/utility/verify.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "solar/math/rect.h"
#include "d3d9_release_com_object.h"
#include "d3d9_verify.h"
#include "d3d9_error.h"
#include "d3d9_render_state_block.h"

namespace solar {

	d3d9_context::d3d9_context()
		: _hwnd(nullptr)
		, _IDirect3D9(nullptr)
		, _IDirect3DDevice9(nullptr)
		, _is_cursor_clipped(false)
		, _changing_device_count(0)
		, _is_device_lost(false)
		, _is_window_active(false)
		, _is_window_minimized(false)
		, _is_window_in_size_move(false)
		, _is_window_in_menu_loop(false)
		, _are_device_objects_created(false)
		, _are_device_objects_reset(false)
		, _current_render_state_flags(0) {

		::ZeroMemory(&_backbuffer_desc, sizeof(D3DSURFACE_DESC));
	}

	d3d9_context::~d3d9_context() {
		ASSERT(_event_handlers.empty());
		teardown();
	}

	bool d3d9_context::setup(HWND hwnd, const d3d9_user_settings& user_settings) {

		_hwnd = hwnd;
		_is_window_active = (::GetForegroundWindow() == hwnd);

		ASSERT(_IDirect3D9 == nullptr);
		_IDirect3D9 = ::Direct3DCreate9(D3D_SDK_VERSION);
		if (_IDirect3D9 == nullptr) {
			ALERT("Direct3DCreate9 failed");
			return false;
		}

		_enumerator.setup(d3d9_enumerator_setup_params()
			.set_IDirect3D9(_IDirect3D9));

		return apply_user_settings(user_settings);
	}

	void d3d9_context::teardown() {
		ASSERT(_event_handlers.empty());
		release_device();
		d3d9_release_com_object(_IDirect3D9);
		_hwnd = nullptr;
	}

	IDirect3DDevice9* d3d9_context::get_device() {
		return _IDirect3DDevice9;
	}

	bool d3d9_context::handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
		UNUSED_PARAMETER(lparam);

		bool handled = false;

		//gracefully handle d3d not attached to any window yet so clients don't have to deal with messages getting
		//sent during to window creation.
		if (_hwnd != nullptr) {
			ASSERT(_hwnd == hwnd);

			switch (message) {
				case WM_SIZE: {
					_is_window_minimized = (wparam == SIZE_MINIMIZED);
					if (!_is_window_minimized) {
						check_if_window_size_changed();
					}
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
					attempt_clip_cursor();
					handled = true;
					break;
				}

				case WM_MOVE: {
					attempt_clip_cursor();
					handled = true;
					break;
				}
			}
		}

		return handled;
	}

	void d3d9_context::check_if_window_size_changed() {
		bool should_check =
			_device_params.get_present_parameters().Windowed &&
			!is_changing_device() &&
			!_is_device_lost &&
			!_is_window_in_size_move;

		if (should_check) {
			if (get_window_client_size() != _device_params.get_backbuffer_size()) {
				d3d9_device_params adjusted_device_params = _device_params;
				adjusted_device_params.set_window_top_left(get_window_top_left());
				adjusted_device_params.get_present_parameters().BackBufferWidth = get_window_client_size()._width;
				adjusted_device_params.get_present_parameters().BackBufferHeight = get_window_client_size()._height;
				change_device(adjusted_device_params);
			}
		}
	}

	void d3d9_context::add_device_event_handler(d3d9_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
		if (_IDirect3DDevice9 != nullptr) {
			handler->on_device_created(_IDirect3DDevice9);
			handler->on_device_reset(_IDirect3DDevice9);
		}
	}

	void d3d9_context::remove_device_event_handler(d3d9_device_event_handler* handler) {
		if (try_find_and_erase(_event_handlers, handler)) {
			if (_IDirect3DDevice9 != nullptr) {
				handler->on_device_lost(_IDirect3DDevice9);
				handler->on_device_released(_IDirect3DDevice9);
			}
		}
	}

	void d3d9_context::attempt_render_scene(render_scene_functor& func) {
		if (_IDirect3DDevice9 != nullptr) {

			bool is_paused =
				_is_window_minimized ||
				_is_window_in_size_move ||
				_is_window_in_menu_loop ||
				is_changing_device();
			if (!is_paused) {
				if (_is_device_lost) {
					HRESULT hr = _IDirect3DDevice9->TestCooperativeLevel();
					if (hr != D3DERR_DEVICELOST) {
						hr = reset_device();
						if (SUCCEEDED(hr)) {
							_is_device_lost = false;
						}
					}
				}
				else {
					func.render_scene();

					HRESULT hr = _IDirect3DDevice9->Present(nullptr, nullptr, nullptr, nullptr);
					if (FAILED(hr)) {
						TRACE("d3d9 present failed : {}", make_d3d9_error_message(hr));
						_is_device_lost = true;
					}
				}
			}
		}
	}

	void d3d9_context::toggle_virtual_fullscreen() {

		TRACE("d3d9_context toggle_virtual_fullscreen...");

		if (_device_params.get_window_type() == d3d9_window_type::VIRTUAL_FULLSCREEN) {
			auto new_params = _device_params;
			new_params.set_window_type(d3d9_window_type::RESIZABLE_WINDOW);
			new_params.get_present_parameters().BackBufferWidth = _exit_fullscreen_backbuffer_size._width;
			new_params.get_present_parameters().BackBufferHeight = _exit_fullscreen_backbuffer_size._height;
			change_device(new_params);
		}
		else if (_device_params.get_window_type() == d3d9_window_type::RESIZABLE_WINDOW) {
			auto new_params = _device_params;
			new_params.set_window_type(d3d9_window_type::VIRTUAL_FULLSCREEN);
			adjust_device_params_back_buffer(new_params);
			change_device(new_params);
		}
		else {
			TRACE("ignoring toggle_virtual_fullscreen due to current window_type being unsupported : {}", to_string(_device_params.get_window_type()));
		}
	}

	unsigned int d3d9_context::get_backbuffer_width() const {
		return _backbuffer_desc.Width;
	}

	unsigned int d3d9_context::get_backbuffer_height() const {
		return _backbuffer_desc.Height;
	}

	size d3d9_context::get_backbuffer_size() const {
		return size(_backbuffer_desc.Width, _backbuffer_desc.Height);
	}

	bool d3d9_context::apply_user_settings(const d3d9_user_settings& user_settings) {

		TRACE("d3d9_context apply user settings...");
		user_settings.trace_dump();

		auto enumerator_device_info = _enumerator.build_best_device_info_from_user_settings(_IDirect3D9, user_settings);
		if (!enumerator_device_info.get_is_valid()) {
			return false;
		}

		_is_cursor_clipped = user_settings.get_is_cursor_clipped();
		_exit_fullscreen_backbuffer_size = user_settings.get_backbuffer_size();

		return change_device(build_device_params(user_settings, enumerator_device_info));
	}

	d3d9_device_params d3d9_context::build_device_params(const d3d9_user_settings& user_settings, const d3d9_enumerator_device_info& enumerator_device_info) const {

		ASSERT(enumerator_device_info.get_is_valid());

		auto device_params = d3d9_device_params()
			.set_window_type(user_settings.get_window_type())
			.set_window_top_left(user_settings.get_window_top_left())
			.set_is_window_constrainted_to_desktop(user_settings.get_is_window_constrainted_to_desktop())
			.set_device_type(enumerator_device_info.get_device_type())
			.set_adapter_ordinal(enumerator_device_info.get_adapter_ordinal())
			.set_adapter_format(enumerator_device_info.get_adapter_format())
			.set_behavior_flags(enumerator_device_info.get_behavior_flags())
			.set_present_parameters(enumerator_device_info.get_present_parameters());
		adjust_device_params_back_buffer(device_params);
		return device_params;
	}

	void d3d9_context::adjust_device_params_back_buffer(d3d9_device_params& device_params) const {
		auto& pp = device_params.get_present_parameters();

		if (device_params.get_window_type() == d3d9_window_type::VIRTUAL_FULLSCREEN) {
			pp.BackBufferWidth = get_desktop_size()._width;
			pp.BackBufferHeight = get_desktop_size()._height;
		}
		else if (device_params.get_window_type() == d3d9_window_type::RESIZABLE_WINDOW) {
			if (is_window_maximized()) {
				pp.BackBufferWidth = get_window_client_size()._width;
				pp.BackBufferHeight = get_window_client_size()._height;
			}
		}
	}

	bool d3d9_context::is_changing_device() const {
		return (_changing_device_count > 0);
	}

	bool d3d9_context::change_device(const d3d9_device_params& params) {

		ASSERT(_hwnd != nullptr);

		TRACE("d3d9_context changing device...");
		params.trace_dump();

		_changing_device_count += 1;

		d3d9_device_params old_device_params = _device_params;

		_device_params = params;
		_device_params.get_present_parameters().hDeviceWindow = _hwnd;

		bool success = false;
		if (process_hwnd_before_device_change(old_device_params)) {
			if (reset_or_recreate_device(old_device_params)) {
				if (process_hwnd_after_device_change(old_device_params)) {
					success = true;
				}
			}
		}

		attempt_clip_cursor();

		_changing_device_count -= 1;

		return success;
	}

	bool d3d9_context::process_hwnd_before_device_change(const d3d9_device_params& old_device_params) {
		if (_device_params.get_window_type() == d3d9_window_type::CHILD_WINDOW) {
			//do nothing, not allowed to touch the HWND for a child window!
		}
		else {
			if (_device_params.get_window_type() != old_device_params.get_window_type()) {

				DWORD new_style = 0;
				if (_device_params.get_window_type() == d3d9_window_type::RESIZABLE_WINDOW) {
					new_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU;
				}
				else if (_device_params.get_window_type() == d3d9_window_type::VIRTUAL_FULLSCREEN) {
					new_style = WS_OVERLAPPED | WS_SYSMENU;
				}
				else if (_device_params.get_window_type() == d3d9_window_type::EXCLUSIVE_FULLSCREEN) {
					new_style = WS_POPUP | WS_SYSMENU;
				}
				else {
					ASSERT(false); //unknown window type
				}

				TRACE("d3d9_window_type is changing GWL_STYLE {{ old_window_type:{} , new_window_type:{} }}"
					, to_string(old_device_params.get_window_type())
					, to_string(_device_params.get_window_type()));

				::ShowWindow(_hwnd, SW_HIDE);
				::ShowWindow(_hwnd, SW_RESTORE);
				::SetWindowLongPtr(_hwnd, GWL_STYLE, new_style);
			}
		}
		return true;
	}

	bool d3d9_context::is_window_maximized() const {
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		VERIFY(::GetWindowPlacement(_hwnd, &placement) == TRUE);
		return (placement.showCmd == SW_SHOWMAXIMIZED);
	}

	size d3d9_context::get_desktop_size() const {
		return size(
			::GetSystemMetrics(SM_CXSCREEN),
			::GetSystemMetrics(SM_CYSCREEN));
	}

	size d3d9_context::get_window_client_size() const {
		RECT client_rect;
		::GetClientRect(_hwnd, &client_rect);
		return size(
			client_rect.right - client_rect.left,
			client_rect.bottom - client_rect.top);
	}

	point d3d9_context::get_window_top_left() const {
		WINDOWPLACEMENT placement;
		placement.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(_hwnd, &placement);
		return point(placement.rcNormalPosition.left, placement.rcNormalPosition.top);
	}

	bool d3d9_context::process_hwnd_after_device_change(const d3d9_device_params& old_device_params) {
		UNUSED_PARAMETER(old_device_params);

		bool success = true;

		if (_device_params.get_window_type() == d3d9_window_type::CHILD_WINDOW) {
			//do nothing, not allowed to touch the HWND for a child window!
		}
		else {
			if (_device_params.get_window_type() != d3d9_window_type::EXCLUSIVE_FULLSCREEN) {

				//going to fullscreen forced the window to be topmost. We never want this in windowed mode so force it back to non-topmost.
				::SetWindowPos(_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);

				if (!is_window_maximized()) {

					RECT new_window_size = { 0 };
					new_window_size.right = _device_params.get_backbuffer_size()._width;
					new_window_size.bottom = _device_params.get_backbuffer_size()._height;
					VERIFY(::AdjustWindowRect(&new_window_size, GetWindowLong(_hwnd, GWL_STYLE), FALSE) == TRUE);

					point new_window_top_left = point(0, 0);
					if (_device_params.get_window_type() == d3d9_window_type::RESIZABLE_WINDOW) {
						new_window_top_left = _device_params.get_window_top_left();
					}

					rect new_window_rect = rect(
						new_window_top_left,
						size(new_window_size.right - new_window_size.left, new_window_size.bottom - new_window_size.top));
					if (_device_params.get_is_window_constrainted_to_desktop()) {
						new_window_rect = make_rect_constrained_within(rect(point(0, 0), get_desktop_size()), new_window_rect);
					}

					::MoveWindow(
						_hwnd,
						new_window_rect.get_left(),
						new_window_rect.get_top(),
						new_window_rect.get_width(),
						new_window_rect.get_height(),
						FALSE);

					//do an extra check after window has been moved/resized. might have been auto adjusted by windows if outside of desktop.
					if (get_window_client_size() != _device_params.get_backbuffer_size()) {
						TRACE("d3d9 changing device to match window_client_size...");
						TRACE("window_client_size : {0}", get_window_client_size());
						TRACE("backbuffer_size : {0}", _device_params.get_backbuffer_size());

						d3d9_device_params adjusted_device_params = _device_params;
						adjusted_device_params.set_window_top_left(new_window_rect.get_top_left());
						adjusted_device_params.get_present_parameters().BackBufferWidth = get_window_client_size()._width;
						adjusted_device_params.get_present_parameters().BackBufferHeight = get_window_client_size()._height;

						success = change_device(adjusted_device_params);
					}
				}
			}
		}

		if (!::IsWindowVisible(_hwnd)) {
			::ShowWindow(_hwnd, SW_SHOW);
		}

		return success;
	}

	bool d3d9_context::reset_or_recreate_device(const d3d9_device_params& old_device_params) {

		bool success = false;

		bool should_reset =
			_IDirect3D9 != nullptr &&
			old_device_params.get_adapter_ordinal() == _device_params.get_adapter_ordinal() &&
			old_device_params.get_device_type() == _device_params.get_device_type() &&
			old_device_params.get_behavior_flags() == _device_params.get_behavior_flags();
		if (should_reset) {

			HRESULT hr = reset_device();
			if (FAILED(hr)) {
				if (hr == D3DERR_DEVICELOST) {
					_is_device_lost = true;
					success = true;
				}
				else {
					release_device();
					success = create_device();
				}
			}
			else {
				success = true;
			}

		}
		else {

			release_device();
			success = create_device();

		}

		return success;
	}

	bool d3d9_context::create_device() {
		ASSERT(_hwnd != nullptr);
		ASSERT(_IDirect3D9 != nullptr);
		ASSERT(_IDirect3DDevice9 == nullptr);

		TRACE("d3d9 create_device : begin");

		HRESULT hr = _IDirect3D9->CreateDevice(
			_device_params.get_adapter_ordinal(),
			_device_params.get_device_type(),
			_hwnd,
			_device_params.get_behavior_flags(),
			&_device_params.get_present_parameters(),
			&_IDirect3DDevice9);
		if (hr == D3DERR_DEVICELOST) {
			_is_device_lost = true;
		}
		else if (SUCCEEDED(hr)) {
			sync_backbuffer_desc_to_device();
			handle_device_created();
			handle_device_reset();
		}

		TRACE("d3d9 create_device : end ({})", make_d3d9_error_message(hr));

		return SUCCEEDED(hr);
	}

	void d3d9_context::release_device() {
		if (_IDirect3DDevice9 != nullptr) {

			TRACE("d3d9 release_device : begin");

			handle_device_lost();
			handle_device_released();
			//steam overlay or gpa seem to sometimes cause unreleased resources.
			ULONG unreleased_resource_count = _IDirect3DDevice9->Release();
			_IDirect3DDevice9 = nullptr;
			::ZeroMemory(&_backbuffer_desc, sizeof(D3DSURFACE_DESC));

			TRACE("d3d9 release_device : end {{ unreleased_resource_count : {0} }}", unreleased_resource_count);
		}
	}

	HRESULT d3d9_context::reset_device() {
		ASSERT(_IDirect3DDevice9 != nullptr);

		TRACE("d3d9 reset_device : begin");

		handle_device_lost();
		HRESULT hr = _IDirect3DDevice9->Reset(&_device_params.get_present_parameters());
		if (SUCCEEDED(hr)) {
			sync_backbuffer_desc_to_device();
			handle_device_reset();
		}
		else {
			TRACE("d3d9 reset_device failed : {}", make_d3d9_error_message(hr));
		}

		TRACE("d3d9 reset_device : end");

		return hr;
	}

	void d3d9_context::sync_backbuffer_desc_to_device() {
		IDirect3DSurface9* backbuffer = nullptr;
		D3D9_VERIFY(_IDirect3DDevice9->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer));
		backbuffer->GetDesc(&_backbuffer_desc);
		d3d9_release_com_object(backbuffer);
	}

	void d3d9_context::handle_device_created() {
		ASSERT(!_are_device_objects_created);
		for (auto event_handler : _event_handlers) {
			event_handler->on_device_created(_IDirect3DDevice9);
		}
		_are_device_objects_created = true;
	}

	void d3d9_context::handle_device_released() {
		if (_are_device_objects_created) {
			for (auto event_handler : _event_handlers) {
				event_handler->on_device_released(_IDirect3DDevice9);
			}
			_are_device_objects_created = false;
		}
	}

	void d3d9_context::handle_device_reset() {
		ASSERT(!_are_device_objects_reset);
		for (auto event_handler : _event_handlers) {
			event_handler->on_device_reset(_IDirect3DDevice9);
		}
		_are_device_objects_reset = true;
	}

	void d3d9_context::handle_device_lost() {
		if (_are_device_objects_reset) {
			for (auto event_handler : _event_handlers) {
				event_handler->on_device_lost(_IDirect3DDevice9);
			}
			_are_device_objects_reset = false;
		}
	}

	void d3d9_context::attempt_clip_cursor() {
		if (_is_cursor_clipped) {
			//don't clip if window is NOT active or else the game could shutdown and leave the cursor badly clipped
			if (_is_window_active) {
				RECT window_rect = { 0 };
				::GetWindowRect(_hwnd, &window_rect);
				::ClipCursor(&window_rect);
			}
		}
		else {
			::ClipCursor(NULL);
		}
	}

	render_state_block* d3d9_context::create_render_state_block(const render_state_block_def& def) {
		return new d3d9_render_state_block(*this, def);
	}

	void d3d9_context::release_render_state_block(render_state_block* block) {
		delete block;
	}

	uint64_t d3d9_context::get_set_current_render_state_flags(uint64_t new_flags) {
		auto old_flags = _current_render_state_flags;
		_current_render_state_flags = new_flags;
		return old_flags;
	}

}