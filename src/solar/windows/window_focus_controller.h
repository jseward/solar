#pragma once

#include <vector>

namespace solar {

	class window;

	enum class window_focus_controller_should_handle_key_down {
		NO,
		YES
	};

	class root_window;

	class window_focus_controller {
	private:
		window& _this_window;
		window_focus_controller_should_handle_key_down _should_handle_key_down;

		root_window* _root_window;
		std::vector<window*> _children;
		int _focused_child_index;

	public:
		window_focus_controller(window& this_window, window_focus_controller_should_handle_key_down should_handle_key_down);
		window_focus_controller(const window_focus_controller&) = delete;
		window_focus_controller& operator=(const window_focus_controller&) = delete;
		~window_focus_controller();

		window& get_this_window() const;

		void add_focusable_child(window* child);
		void remove_focusable_child(window* child);

		window* get_focused_child() const;
		void set_focused_child(window* child, bool should_apply_changes_to_previous_focused_child);

		void move_to_front();
	};

}
