#include "root_window.h"

#include "solar/utility/assert.h"
#include "solar/rendering/render_device.h"
#include "solar/containers/container_helpers.h"

namespace solar {

	root_window* root_window::_s_instance = nullptr;

	void root_window::set(root_window* instance) {
		_s_instance = instance;
	}

	root_window& root_window::get() {
		ASSERT(_s_instance != nullptr);
		return *_s_instance;
	}

	root_window::root_window()
		: window("root")
		, _window_under_cursor_while_rendering(nullptr) {
	}

	root_window::~root_window() {
		ASSERT(_window_under_cursor_while_rendering == nullptr);
		ASSERT(_focus_controllers.empty());
	}

	window_focus_controller* root_window::get_active_focus_controller() const {
		for (auto fc : _focus_controllers) {
			if (
				fc->get_this_window().is_enabled() &&
				fc->get_this_window().is_visible_recursive()) {
				return fc;
			}
		}
		return nullptr;
	}

	void root_window::on_child_added(window* child) {
		add_focus_controller_recursive(child);
	}

	void root_window::on_child_removed(window* child) {
		remove_focus_controller_recursive(child);
	}

	void root_window::add_focus_controller_recursive(window* window) {
		if (window->as_focus_controller() != nullptr) {
			push_back_and_verify_not_found(_focus_controllers, window->as_focus_controller());
		}

		for (auto c : window->get_children()) {
			add_focus_controller_recursive(c);
		}
	}

	void root_window::remove_focus_controller_recursive(window* window) {
		if (window->as_focus_controller() != nullptr) {
			find_and_erase(_focus_controllers, window->as_focus_controller());
		}

		for (auto c : window->get_children()) {
			remove_focus_controller_recursive(c);
		}
	}

	void root_window::render_all(const window_render_params& params) {
		_window_under_cursor_while_rendering = get_window_under_cursor_recursive(params._cursor_pos, *this);
		render_recursive(params, *this, *this);
		_window_under_cursor_while_rendering = nullptr;
	}

	bool root_window::root_on_mouse_button_down(const window_mouse_button_event_params& params, bool is_trapped_externally) {
		bool is_trapped = false;

		if (!is_trapped_externally) {
			//NOTE: not checking is_enabled here, windows do that internally so they can trap the event
			auto filter = [&params](window& w) { return w.is_visible() && w.get_area().is_point_within(params._cursor_pos); };
			auto handler = [&params](window& w) { return w.on_mouse_button_down(params); };
			if (handle_event_recursive(filter, handler, *this)) {
				is_trapped = true;
			}
		}

		return is_trapped;
	}

	bool root_window::root_on_mouse_button_up(const window_mouse_button_event_params& params, bool is_trapped_externally) {
		bool is_trapped = false;

		if (!is_trapped_externally) {
			//NOTE: not checking is_enabled here, windows do that internally so they can trap the event EVEN when disabled 
			//(ex. button still blocks mouse button presses even when disabled)
			auto filter = [&params](window& w) { return w.is_visible() && w.get_area().is_point_within(params._cursor_pos); };
			auto handler = [&params](window& w) { return w.on_mouse_button_up(params); };
			if (handle_event_recursive(filter, handler, *this)) {
				is_trapped = true;
			}
		}

		{
			auto anywhere_filter = [](window&) { return true; };
			auto anywhere_handler = [&params](window& w) { return w.on_mouse_button_up_anywhere(params); };
			if (handle_event_recursive(anywhere_filter, anywhere_handler, *this)) {
				is_trapped = true;
			}
		}

		return is_trapped;
	}

	bool root_window::root_on_key_down(const window_key_event_params& params, bool is_trapped_externally) {
		bool is_trapped = false;

		if (!is_trapped_externally) {
			auto filter = [&params](window& w) { return w.is_visible() && w.is_enabled(); };
			auto handler = [&params](window& w) { return w.on_key_down(params); };
			if (handle_event_recursive(filter, handler, *this)) {
				is_trapped = true;
			}
		}

		{
			auto anywhere_filter = [](window&) { return true; };
			auto anywhere_handler = [&params](window& w) { return w.on_key_down_anywhere(params); };
			if (handle_event_recursive(anywhere_filter, anywhere_handler, *this)) {
				is_trapped = true;
			}
		}

		return is_trapped;
	}

	bool root_window::root_on_char_received(const window_char_event_params& params) {
		auto handler = [&params](window& w) { return w.on_char_received(params); };
		return handle_event_focus_only(handler);
	}

	window* root_window::get_window_under_cursor_recursive(const point& cursor_pos, window& current_window) {
		if (current_window.is_visible()) {
			for (auto child_iter = current_window.get_children().rbegin(); child_iter != current_window.get_children().rend(); ++child_iter) {
				auto child_window_under_cursor = get_window_under_cursor_recursive(cursor_pos, **child_iter);
				if (child_window_under_cursor != nullptr) {
					return child_window_under_cursor;
				}
			}

			if (current_window.can_be_under_cursor() && current_window.get_area().is_point_within(cursor_pos)) {
				return &current_window;
			}
		}

		return nullptr;
	}

	void root_window::render_recursive(const window_render_params& params, const window& root_window, window& current_window) {
		if (current_window.is_visible()) {
			if (!current_window.will_clip_viewport()) {
				render_recursive_within_viewport(params, root_window, current_window);
			}
			else {
				auto safe_viewport_area = make_rect_constrained_within(root_window.get_area(), current_window.get_area());
				auto old_viewport = params._render_device.set_viewport(viewport().set_area(safe_viewport_area));
				render_recursive_within_viewport(params, root_window, current_window);
				params._render_device.set_viewport(old_viewport);
			}
		}
	}

	void root_window::render_recursive_within_viewport(const window_render_params& params, const window& root_window, window& current_window) {
		current_window.render(params);
		for (auto child_iter = current_window.get_children().begin(); child_iter != current_window.get_children().end(); ++child_iter) {
			render_recursive(params, root_window, **child_iter);
		}
		current_window.render_after_children(params);
	}

}