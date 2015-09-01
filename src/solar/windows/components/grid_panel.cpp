#include "grid_panel.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	grid_panel_row::grid_panel_row()
		: _height(0) {
	}

	void grid_panel_row::read_from_archive(archive_reader& reader) {
		read_int(reader, "height", _height);
	}

	void grid_panel_row::write_to_archive(archive_writer& writer) const {
		write_int(writer, "height", _height);
	}

	grid_panel_column::grid_panel_column()
		: _width(0) {
	}

	void grid_panel_column::read_from_archive(archive_reader& reader) {
		read_int(reader, "width", _width);
	}

	void grid_panel_column::write_to_archive(archive_writer& writer) const {
		write_int(writer, "width", _width);
	}

	grid_panel::grid_panel(const char* id) 
		: window_component(id) {
	}

	grid_panel::~grid_panel() {
	}

	void grid_panel::on_child_added(window* child) {
		ASSERT(child->as_component() != nullptr);
		child->as_component()->set_has_grid_position(true);
	}

	void grid_panel::on_area_changed() {
		int current_top = get_area().get_top();
		for (unsigned int row_index = 0; row_index < _rows.size(); ++row_index) {
			auto& row = _rows.at(row_index);
			int scaled_height = float_to_int(row._height * get_area_scale());
			if (row_index == _rows.size() - 1) {
				scaled_height = (get_area().get_bottom() - current_top);
			}
			row._current_area = rect(point(get_area().get_left(), current_top), size(get_area().get_width(), scaled_height));
			current_top += scaled_height;
			current_top = std::min(current_top, get_area().get_bottom());
		}

		int current_left = get_area().get_left();
		for (unsigned int column_index = 0; column_index < _columns.size(); ++column_index) {
			auto& column = _columns.at(column_index);
			int scaled_width = float_to_int(column._width * get_area_scale());
			if (column_index == _columns.size() - 1) {
				scaled_width = (get_area().get_right() - current_left);
			}
			column._current_area = rect(point(current_left, get_area().get_top()), size(scaled_width, get_area().get_height()));
			current_left += scaled_width;
			current_left = std::min(current_left, get_area().get_right());
		}

		for (auto& child : get_children()) {
			const point& grid_position = child->as_component()->get_grid_position();
			IF_VERIFY(grid_position._x < static_cast<int>(_rows.size()) && grid_position._y < static_cast<int>(_columns.size())) {
				const auto& row = _rows.at(grid_position._x);
				const auto& column = _columns.at(grid_position._y);
				child->set_area(rect(
					point(column._current_area.get_left(), row._current_area.get_top()),
					point(column._current_area.get_right(), row._current_area.get_bottom())));
			}
		}
	}

	void grid_panel::read_from_archive(archive_reader& reader) {
		window_component::read_from_archive(reader);	
		read_object_vector(reader, "rows", _rows);
		read_object_vector(reader, "columns", _columns);
	}

	void grid_panel::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_object_vector(writer, "rows", _rows);
		write_object_vector(writer, "columns", _columns);
	}

}


