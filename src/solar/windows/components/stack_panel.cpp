#include "stack_panel.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	stack_panel::stack_panel(const char* id, stack_panel_orientation orientation)
		: window_component(id)
		, _orientation(orientation) {
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
	}

	void stack_panel::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
	}

	void stack_panel::update_child_areas_horizontal() {
		auto& children = get_children();
		if (!children.empty()) {
			int child_width = get_area().get_width() / children.size();
			point child_top_left = get_area().get_top_left();
			size child_size = size(child_width, get_area().get_size()._height);
			for (auto& child : children) {
				child->as_component()->update_area(rect(child_top_left, child_size));
				child_top_left._x += child_width;
			}
		}
	}

	void stack_panel::update_child_areas_vertical() {
		auto& children = get_children();
		if (!children.empty()) {
			int child_height = get_area().get_height() / children.size();
			point child_top_left = get_area().get_top_left();
			size child_size = size(get_area().get_size()._width, child_height);
			for (auto& child : children) {
				child->set_area(rect(child_top_left, child_size));
				child_top_left._y += child_height;
			}
		}
	}

}