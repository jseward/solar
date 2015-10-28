#include "window.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_build.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window::window(const char* id)
		: _id(id)
		, _area()
		, _alpha(1.f)
		, _is_visible(true)
		, _can_be_under_cursor(true)
		, _will_clip_viewport(false)
		, _parent(nullptr)
		, _parent_focus_controller(nullptr)
		, _are_children_locked(false) {
	}

	window::~window() {
		ASSERT(_parent == nullptr);
		ASSERT(_parent_focus_controller == nullptr);
		ASSERT(_children.empty());
	}

	const char* window::get_id() const {
		return _id;
	}

	void window::set_is_visible_callback(is_visible_callback_function is_visible_callback) {
		_is_visible_callback = is_visible_callback;
	}

	void window::set_is_enabled_callback(is_enabled_callback_function is_enabled_callback) {
		_is_enabled_callback = is_enabled_callback;
	}

	void window::lock_children() {
		ASSERT(!_are_children_locked);
		_are_children_locked = true;
	}

	void window::unlock_children() {
		ASSERT(_are_children_locked);
		_are_children_locked = false;
	}

	void window::add_child(window* child) {
		ASSERT(!_are_children_locked);
		push_back_and_verify_not_found(_children, child);		
		ASSERT(child->_parent == nullptr);
		child->_parent = this;
		child->try_attach_self_and_children_to_best_focus_controller();

		on_child_added(child);

		//NOT calling on_parent_area_changed because there are too many side effects. add_child is usually done
		//in constructors when nothing is loaded so area changes can't be properly handled. clients need hacky code
		//to ignore the area change event if nothing loaded...
		//
		//child->on_parent_area_changed();
	}

	void window::remove_child(window* child) {
		ASSERT(!_are_children_locked);
		find_and_erase(_children, child);
		ASSERT(child->_parent == this);
		child->_parent = nullptr;
		child->try_detach_self_and_children_from_focus_controller();

		on_child_removed(child);
	}

	void window::remove_all_children() {
		while (!_children.empty()) {
			remove_child(*_children.begin());
		}
	}

	const std::vector<window*>& window::get_children() const {
		return _children;
	}

	bool window::has_parent() const {
		return (_parent != nullptr);
	}

	window& window::get_parent() const {
		ASSERT(_parent != nullptr);
		return *_parent;
	}

	void window::set_area(const rect& new_area) {
		_area = new_area;
		on_area_changed();

		for (auto child : _children) {
			child->on_parent_area_changed();
		}
	}

	const rect& window::get_area() const {
		return _area;
	}

	float window::get_area_scale() const {
		if (_parent != nullptr) {
			return _parent->get_area_scale();
		}
		return 1.f;
	}

	float window::get_font_scale() const {
		if (_parent != nullptr) {
			return _parent->get_font_scale();
		}
		return 1.f;
	}

	float window::get_alpha() const {
		return _alpha;
	}

	bool window::is_enabled() const {
		if (_is_enabled_callback != nullptr) {
			return _is_enabled_callback();
		}
		return true;
	}

	bool window::is_focused() const {
		if (_parent_focus_controller != nullptr) {
			return (_parent_focus_controller->get_focused_child() == this);
		}
		return false;
	}

	void window::set_is_visible(bool is_visible) {
		ASSERT(_is_visible_callback == nullptr);
		if (_is_visible != is_visible) {
			_is_visible = is_visible;
			on_is_visible_changed();
		}
	}

	bool window::is_visible() const {
		if (_is_visible_callback != nullptr) {
			return _is_visible_callback();
		}
		return _is_visible;
	}

	bool window::is_visible_recursive() const {
		const window* w = this;
		while (w != nullptr) {
			if (!w->is_visible()) {
				return false;
			}
			w = w->_parent;
		}
		return true;
	}

	bool window::will_clip_viewport() const {
		return _will_clip_viewport;
	}

	void window::set_is_focus_controller(window_focus_controller_should_handle_key_down should_handle_key_down) {
		_this_focus_controller = std::make_unique<window_focus_controller>(*this, should_handle_key_down);
	}

	window_focus_controller* window::as_focus_controller() {
		return _this_focus_controller.get();
	}

	void window::try_make_focused() {
		ASSERT(is_focusable_ever());
		if (_parent_focus_controller != nullptr) {
			if (is_focusable_now()) {
				_parent_focus_controller->set_focused_child(this, true);
			}
		}
	}

	bool window::is_focusable_now() const {
		return is_enabled() && is_visible_recursive();
	}

	bool window::can_be_under_cursor() const {
		return _can_be_under_cursor;
	}

	void window::on_is_visible_changed() {
	}

	void window::render(const window_render_params& params) {
		UNUSED_PARAMETER(params);
	}

	void window::render_after_children(const window_render_params& params) {
		UNUSED_PARAMETER(params);
	}

	window_component* window::as_component() {
		return nullptr;
	}

	void window::on_loaded() {
	}

	void window::on_parent_area_changed() {
	}

	void window::on_area_changed() {
	}
	
	void window::on_child_added(window* child) {
		UNUSED_PARAMETER(child);
	}

	void window::on_child_removed(window* child) {
		UNUSED_PARAMETER(child);
	}

	bool window::on_mouse_button_down(const window_mouse_button_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::on_mouse_button_up(const window_mouse_button_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::on_key_down(const window_key_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::on_key_down_anywhere(const window_key_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::on_char_received(const window_char_event_params& params) {
		UNUSED_PARAMETER(params);
		return false;
	}

	bool window::is_focusable_ever() const {
		return false;
	}

	void window::on_focus_lost(bool should_apply_changes) {
		UNUSED_PARAMETER(should_apply_changes);
	}

	void window::on_focus_gained() {
	}

	void window::read_from_archive(archive_reader& reader) {
		UNUSED_PARAMETER(reader);
	}
	
	void window::write_to_archive(archive_writer& writer) const {
		UNUSED_PARAMETER(writer);
	}

	void window::try_attach_self_and_children_to_best_focus_controller() {
		//NOTE: this is a bit tricky. We need to handle the case where A(focusable) is added to B(not-focusable). Then
		//later B is added to C(focus controller). A should be magically attached to the C focus controller.

		if (is_focusable_ever() && _parent_focus_controller == nullptr) {
			window* w = this;
			while (w != nullptr) {
				_parent_focus_controller = w->_this_focus_controller.get();
				if (_parent_focus_controller != nullptr) {
					_parent_focus_controller->add_focusable_child(this);
					break;
				}
				w = w->_parent;
			}
		}

		for (auto child : _children) {
			child->try_attach_self_and_children_to_best_focus_controller();
		}
	}

	void window::try_detach_self_and_children_from_focus_controller() {
		if (_parent_focus_controller != nullptr) {
			_parent_focus_controller->remove_focusable_child(this);
			_parent_focus_controller = nullptr;
		}

		for (auto child : _children) {
			child->try_detach_self_and_children_from_focus_controller();
		}
	}

}