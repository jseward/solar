#include "window_component.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	window_component::window_component(const char* id) 
		: window(id)
		, _has_layout(true)
		, _has_grid_position(false) {
	}
	
	window_component::~window_component() {
	}

	bool window_component::get_has_layout() const {
		return _has_layout;
	}

	void window_component::set_has_layout(bool has_layout) {
		_has_layout = has_layout;
	}

	void window_component::set_has_grid_position(bool has_grid_position) {
		if (has_grid_position) {
			_has_layout = false;
			_has_grid_position = true;
		}
		else {
			ASSERT(false); //not expected?
		}
	}

	const grid_panel_position& window_component::get_grid_position() const {
		ASSERT(_has_grid_position);
		return _grid_position;
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
		if (_has_layout) {
			set_area(_layout.build_area(get_parent()));
		}
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
		ASSERT(false);
		UNUSED_PARAMETER(writer);
	}

}