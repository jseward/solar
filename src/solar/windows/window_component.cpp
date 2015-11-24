#include "window_component.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	window_component::window_component(const char* id) 
		: window(id)
		, _has_layout(true)
		, _has_grid_position(false)
		, _will_self_update_area(true) {
	}
	
	window_component::~window_component() {
	}

	bool window_component::has_layout() const {
		return _has_layout;
	}

	void window_component::set_has_layout(bool has_layout) {
		_has_layout = has_layout;
	}

	void window_component::set_has_grid_position(bool has_grid_position) {
		_has_grid_position = has_grid_position;
	}

	const grid_panel_position& window_component::get_grid_position() const {
		ASSERT(_has_grid_position);
		return _grid_position;
	}

	void window_component::set_will_self_update_area(bool will_self_update_area) {
		_will_self_update_area = will_self_update_area;
		if (_will_self_update_area) {
			ASSERT(_has_layout);
		}
	}

	const window_layout& window_component::get_layout() const {
		ASSERT(_has_layout);
		return _layout;
	}

	void window_component::on_loaded() {
		try_update_area();
	}

	void window_component::on_parent_area_changed() {
		try_update_area();
	}

	window_component* window_component::as_component() {
		return this;
	}

	void window_component::try_update_area() {
		if (_will_self_update_area) {
			update_area(get_parent().get_area());
		}
	}

	void window_component::update_area(const rect& parent_area) {
		ASSERT(_has_layout);
		set_area(_layout.build_area(parent_area, get_area_scale()));
	}

	void window_component::read_from_archive(archive_reader& reader) {
		if (_has_layout) {
			read_object(reader, "layout", _layout);
		}
		if (_has_grid_position) {
			read_object(reader, "grid_position", _grid_position);
		}
	}

	void window_component::write_to_archive(archive_writer& writer) const {
		if (_has_layout) {
			write_object(writer, "layout", _layout);
		}
		if (_has_grid_position) {
			write_object(writer, "grid_position", _grid_position);
		}
	}

}