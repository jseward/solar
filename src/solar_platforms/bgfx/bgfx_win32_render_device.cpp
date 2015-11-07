#include "bgfx_win32_render_device.h"

#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	bgfx_win32_render_device::bgfx_win32_render_device() 
		: _hwnd(nullptr)
		, _is_window_active(false)
		, _is_window_minimized(false)
		, _is_window_in_size_move(false) 
		, _is_window_in_menu_loop(false) {
	}

	bgfx_win32_render_device::~bgfx_win32_render_device() {
		ASSERT(_event_handlers.empty());
	}

	bool bgfx_win32_render_device::setup(HWND hwnd) {
		_hwnd = hwnd;
		_is_window_active = (::GetForegroundWindow() == hwnd);

		bgfx::winSetHwnd(hwnd);

		if (!bgfx::init()) {
			ALERT("bgfx::init() failed.");
			return false;
		}

		bgfx::reset(640, 480, BGFX_RESET_VSYNC);

		return true;
	}

	void bgfx_win32_render_device::teardown() {
		bgfx::shutdown();
	}

	bool bgfx_win32_render_device::handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
		//todo
		UNUSED_PARAMETER(hwnd);
		UNUSED_PARAMETER(message);
		UNUSED_PARAMETER(wparam);
		UNUSED_PARAMETER(lparam);
		return false;
	}

	void bgfx_win32_render_device::add_event_handler(render_device_event_handler* handler) {
		push_back_and_verify_not_found(_event_handlers, handler);
		handler->on_render_device_surface_size_changed(get_backbuffer_size());
	}

	void bgfx_win32_render_device::remove_event_handler(render_device_event_handler* handler) {
		find_and_erase(_event_handlers, handler);
	}

	void bgfx_win32_render_device::attempt_render_scene(render_scene_functor& func) {
		//todo
		UNUSED_PARAMETER(func);
	}

	bool bgfx_win32_render_device::begin_scene() {
		//todo
		return false;
	}

	void bgfx_win32_render_device::end_scene() {
		//todo
	}

	void bgfx_win32_render_device::clear(const color& color) {
		//todo
		UNUSED_PARAMETER(color);
	}

	void bgfx_win32_render_device::toggle_virtual_fullscreen() {
		//todo
	}

	viewport bgfx_win32_render_device::set_viewport(const viewport& new_viewport) {
		//todo
		UNUSED_PARAMETER(new_viewport);
		return viewport();
	}

	size bgfx_win32_render_device::get_backbuffer_size() const {
		//todo
		return size();
	}

}