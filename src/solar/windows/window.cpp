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
		, _are_children_locked(false) {
	}

	window::~window() {
		ASSERT(_parent == nullptr);
		ASSERT(_children.empty());
	}

	const char* window::get_id() const {
		return _id;
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

		on_child_added(child);
		child->on_parent_area_changed();
	}

	void window::remove_child(window* child) {
		ASSERT(!_are_children_locked);
		find_and_erase(_children, child);
		ASSERT(child->_parent == this);
		child->_parent = nullptr;
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
		return 1.f; //todo
	}

	float window::get_font_scale() const {
		return 1.f;//todo
	}

	float window::get_alpha() const {
		return _alpha;
	}

	bool window::is_enabled() const {
		return true; //todo
	}

	bool window::is_focused() const {
		return false; //todo
	}

	bool window::is_visible() const {
		return _is_visible;
	}

	bool window::will_clip_viewport() const {
		return _will_clip_viewport;
	}

	bool window::can_be_under_cursor() const {
		return _can_be_under_cursor;
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

	void window::read_from_archive(archive_reader& reader) {
		UNUSED_PARAMETER(reader);
	}
	
	void window::write_to_archive(archive_writer& writer) const {
		UNUSED_PARAMETER(writer);
	}

}