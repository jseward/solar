#pragma once

//http://stackoverflow.com/questions/11040133/what-does-defining-win32-lean-and-mean-exclude-exactly
#define VC_EXTRALEAN
#define	WIN32_LEAN_AND_MEAN
#define NOMINMAX //window header files define min() / max() as macros, causes conflicts with 3rdParty libs (ex. lua)
#include <windows.h>

#include <vector>
#include "solar/rendering/render_device.h"

namespace solar {

	class bgfx_win32_render_device : public render_device 
	{
	private:
		HWND _hwnd;
		bool _is_window_active;
		bool _is_window_minimized;
		bool _is_window_in_size_move;
		bool _is_window_in_menu_loop;

		std::vector<render_device_event_handler*> _event_handlers;

	public:
		bgfx_win32_render_device();
		virtual ~bgfx_win32_render_device();

		bool setup(HWND hwnd);
		void teardown();
		bool handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		virtual void add_event_handler(render_device_event_handler* handler) override;
		virtual void remove_event_handler(render_device_event_handler* handler) override;
		virtual void attempt_render_scene(render_scene_functor& func) override;
		virtual bool begin_scene() override;
		virtual void end_scene() override;
		virtual void clear(const color& color) override;
		virtual void toggle_virtual_fullscreen();
		virtual viewport set_viewport(const viewport& new_viewport) override;

	private:
		size get_backbuffer_size() const;
	};

}