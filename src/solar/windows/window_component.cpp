#include "window_component.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	window_component::window_component(const char* id) 
		: window(id)
		, _has_layout(true) {
	}
	
	window_component::~window_component() {
	}

	void window_component::on_loaded() {
		update_area();
	}

	void window_component::on_parent_area_changed() {
		update_area();
	}

	void window_component::update_area() {
		if (_has_layout) {
			set_area(_layout.build_area(get_parent()));
		}
	}

	void window_component::read_from_archive(archive_reader& reader) {
		if (_has_layout) {
			read_object(reader, "layout", _layout);
		}
	}

	void window_component::write_to_archive(archive_writer& writer) const {
		ASSERT(false);
		UNUSED_PARAMETER(writer);
	}

	void window_component::set_has_layout(bool has_layout) {
		_has_layout = has_layout;
	}

}