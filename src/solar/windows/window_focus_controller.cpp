#include "window_focus_controller.h"

#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/utility/type_convert.h"
#include "solar/containers/container_helpers.h"
#include "window.h"
#include "root_window.h"

namespace solar {

	window_focus_controller::window_focus_controller(window& this_window, window_focus_controller_should_handle_key_down should_handle_key_down) 
		: _this_window(this_window)
		, _should_handle_key_down(should_handle_key_down) 
		, _root_window(nullptr)
		, _focused_child_index(0) {
	}

	window_focus_controller::~window_focus_controller() {
		ASSERT(_children.empty())
	}

	window& window_focus_controller::get_this_window() const {
		return _this_window;
	}

	void window_focus_controller::add_focusable_child(window* child) {
		push_back_and_verify_not_found(_children, child);
	}

	void window_focus_controller::remove_focusable_child(window* child) {
		find_and_erase(_children, child);
	}

	window* window_focus_controller::get_focused_child() const {
		int child_count = uint_to_int(_children.size());
		if (child_count == 0) {
			return nullptr;
		}

		window* child = _children.at(_focused_child_index % child_count);
		if (!child->is_focusable_now()) {
			return nullptr;
		}

		return child;
	}

	void window_focus_controller::set_focused_child(window* child, bool should_apply_changes_to_previous_focused_child) {
		if (child != nullptr) {
			ASSERT(child->is_focusable_now());
		}

		int child_index = find_index(_children, child);
		if (child_index != _focused_child_index) {
			window* old_child = get_focused_child();
			_focused_child_index = child_index;

			TRACE("window focused child changed. window_id:{} , old_child:{} , new_child:{}",
				_this_window.get_id(),
				(old_child != nullptr) ? old_child->get_id() : "null",
				(child != nullptr) ? child->get_id() : "null");

			if (old_child != nullptr) {
				old_child->on_focus_lost(should_apply_changes_to_previous_focused_child);
			}

			if (child != nullptr) {
				child->on_focus_gained();
			}
		}
	}

	void window_focus_controller::move_to_front() {
		ASSERT(_root_window != nullptr);
		_root_window->move_focus_controller_to_front(this);
	}

}
