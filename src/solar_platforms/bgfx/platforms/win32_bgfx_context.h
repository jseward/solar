#pragma once

//http://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define VC_EXTRALEAN
#define	WIN32_LEAN_AND_MEAN
#define NOMINMAX //window header files define min() / max() as macros, causes conflicts with 3rdParty libs (ex. lua)
#include <windows.h>

#include <vector>
#include "../bgfx_context.h"
#include "../bgfx_user_settings.h"
#include "../bgfx_callback.h"

namespace solar {

	class win32_bgfx_context : public bgfx_context {
	private:
		bgfx_callback _bgfx_callback;
		HWND _hwnd;
		bool _is_window_active;
		bool _is_window_minimized;
		bool _is_window_in_size_move;
		bool _is_window_in_menu_loop;
		bgfx_user_settings _user_settings;
		size _backbuffer_size;
		std::vector<backbuffer_size_changed_callback> _backbuffer_size_changed_callbacks;

	public:
		win32_bgfx_context();
		virtual ~win32_bgfx_context();

		bool setup(HWND hwnd, const bgfx_user_settings& settings);
		void teardown();
		bool handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		bool apply_user_settings(const bgfx_user_settings& settings);

		virtual void add_backbuffer_size_changed_callback(backbuffer_size_changed_callback callback) override;
		virtual void toggle_virtual_fullscreen() override;
		virtual size get_backbuffer_size() const override;
		virtual void attempt_render_scene(render_scene_functor& func) override;

	private:
		size get_desktop_size() const;
		size get_window_client_size() const;
		void reset_bgfx(const size& new_backbuffer_size);
		void check_if_window_size_changed();
	};

}
