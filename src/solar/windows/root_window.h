#pragma once

#include "window.h"
#include "window_render_params.h"

namespace solar {

	class root_window : public window {
	private:
		static root_window* _s_instance;

	public:
		static void set(root_window* instance);
		static root_window& get();

	private:
		window* _window_under_cursor_while_rendering;

	public:
		root_window();
		virtual ~root_window();

		void render_all(const window_render_params& params);
		bool root_on_mouse_button_down(const window_mouse_button_event_params& params, bool is_trapped_externally);
		bool root_on_mouse_button_up(const window_mouse_button_event_params& params, bool is_trapped_externally);

	private:
		static window* get_window_under_cursor_recursive(const point& cursor_pos, window& current_window);
		static void render_recursive(const window_render_params& params, const window& root_window, window& current_window);
		static void render_recursive_within_viewport(const window_render_params& params, const window& root_window, window& current_window);
		
		template<typename FilterFunc, typename HandlerFunc>
		static bool handle_event_recursive(FilterFunc filter, HandlerFunc handler, window& window);
	};

	template<typename FilterFunc, typename HandlerFunc>
	bool root_window::handle_event_recursive(FilterFunc filter, HandlerFunc handler, window& window) {
		bool is_trapped_by_self = false;
		bool is_trapped_by_child = false;

		if (filter(window)) {
			//reverse iterate so children rendered on top get event first
			for (auto child_iter = window.get_children().rbegin(); child_iter != window.get_children().rend(); ++child_iter) {
				if (handle_event_recursive(filter, handler, **child_iter)) {
					is_trapped_by_child = true;
					break;
				}
			}

			if (!is_trapped_by_child) {
				if (handler(window)) {
					is_trapped_by_self = true;
				}
			}
		}

		return (is_trapped_by_self || is_trapped_by_child);
	}

}