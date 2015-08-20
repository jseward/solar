#pragma once

#include "d3d9_headers.h"
#include "d3d9_user_settings.h"
#include "d3d9_device_params.h"
#include "d3d9_enumerator.h"
#include "d3d9_device_event_handler.h"
#include "d3d9_cursor_icon.h"
#include "solar/math/rect.h"
#include "solar/rendering/render_scene_functor.h"

namespace solar {
	
	class d3d9_context {

	private:
		HWND _hwnd;
		IDirect3D9* _IDirect3D9;
		IDirect3DDevice9* _IDirect3DDevice9;
		d3d9_enumerator _enumerator;
		bool _is_cursor_clipped;
		int _changing_device_count;
		d3d9_device_params _device_params;
		bool _is_device_lost;
		bool _is_window_active;
		bool _is_window_minimized;
		bool _is_window_in_size_move;
		bool _is_window_in_menu_loop;
		D3DSURFACE_DESC _backbuffer_desc;
		std::vector<d3d9_device_event_handler*> _event_handlers;
		bool _are_device_objects_created;
		bool _are_device_objects_reset;
		size _toggle_from_fullscreen_backbuffer_size;
		d3d9_cursor_icon* _active_cursor_icon;

	public:
		d3d9_context();
		~d3d9_context();

		bool setup(HWND hwnd, const d3d9_user_settings& user_settings);
		void teardown();
		bool handle_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		bool apply_user_settings(const d3d9_user_settings& user_settings);
		void attempt_render_scene(render_scene_functor& func);
		void toggle_virtual_fullscreen();
		unsigned int get_backbuffer_width() const;
		unsigned int get_backbuffer_height() const;
		size get_backbuffer_size() const;

		void add_device_event_handler(d3d9_device_event_handler* handler);
		void remove_device_event_handler(d3d9_device_event_handler* handler);

		IDirect3DDevice9* get_device();

	private:
		bool is_changing_device() const;
		bool change_device(const d3d9_device_params& params);
		bool process_hwnd_before_device_change(const d3d9_device_params& old_device_params);
		bool process_hwnd_after_device_change(const d3d9_device_params& old_device_params);
		d3d9_device_params build_device_params(const d3d9_user_settings& user_settings, const d3d9_enumerator_device_info& enumerator_device_info) const;
		bool reset_or_recreate_device(const d3d9_device_params& old_device_params);
		bool create_device();
		void release_device();
		HRESULT reset_device();
		void handle_device_created();
		void handle_device_released();
		void handle_device_lost();
		void handle_device_reset();
		void sync_backbuffer_desc_to_device();
		bool is_window_maximized() const;
		size get_desktop_size() const;
		size get_window_client_size() const;
		point get_window_top_left() const;
		void attempt_clip_cursor();
		bool attempt_set_cursor();
		void set_is_minimized(bool is_minimized);
		void check_if_window_size_changed();
		void adjust_device_params_back_buffer(d3d9_device_params& device_params) const;
	};

}