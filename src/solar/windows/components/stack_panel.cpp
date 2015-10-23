#include "stack_panel.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	stack_panel::stack_panel(const char* id, stack_panel_orientation orientation)
		: window_component(id)
		, _orientation(orientation) 
		, _cell_size(0)
		, _cell_spacing(0) {
	}

	stack_panel::~stack_panel() {
	}

	void stack_panel::on_area_changed() {
		if (_orientation == stack_panel_orientation::HORIZONTAL) {
			update_child_areas_horizontal();
		}
		else if (_orientation == stack_panel_orientation::VERTICAL) {
			update_child_areas_vertical();
		}
		else {
			ASSERT(false);
		}
	}

	void stack_panel::on_child_added(window* child) {
		auto child_component = child->as_component();
		IF_VERIFY(child_component != nullptr) {
			child_component->set_will_self_update_area(false);
		}
	}

	void stack_panel::read_from_archive(archive_reader& reader) {
		window_component::read_from_archive(reader);
		read_int(reader, "cell_size", _cell_size);
		read_int(reader, "cell_spacing", _cell_spacing);
	}

	void stack_panel::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_int(writer, "cell_size", _cell_size);
		write_int(writer, "cell_spacing", _cell_spacing);
	}

	void stack_panel::update_child_areas_horizontal() {
		auto& children = get_children();
		if (!children.empty()) {
			const int width = float_to_int(_cell_size * get_area_scale());
			const int stride = float_to_int((_cell_size + _cell_spacing) * get_area_scale());
			
			point top_left = get_area().get_top_left();
			for (auto& child : children) {
				child->as_component()->update_area(rect(top_left, size(width, get_area().get_size()._height)));
				top_left._x += stride;
			}
		}
	}

	void stack_panel::update_child_areas_vertical() {
		auto& children = get_children();
		if (!children.empty()) {
			const int height = float_to_int(_cell_size * get_area_scale());
			const int stride = float_to_int((_cell_size + _cell_spacing) * get_area_scale());

			point top_left = get_area().get_top_left();
			for (auto& child : children) {
				child->as_component()->update_area(rect(top_left, size(get_area().get_size()._width, height)));
				top_left._y += stride;
			}
		}
	}

}