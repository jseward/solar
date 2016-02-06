#include "grid_panel.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/containers/container_helpers.h"
#include "solar/archiving/archiving_helpers.h"

namespace solar {

	grid_panel::row_column::row_column() {
	}

	void grid_panel::row_column::read_from_archive(archive_reader& reader) {
		read_optional_int(reader, "has_size", "size", _size);
	}

	void grid_panel::row_column::write_to_archive(archive_writer& writer) const {
		write_optional_int(writer, "has_size", "size", _size);
	}

	grid_panel::grid_panel(const char* id)
		: window_component(id) {
	}

	grid_panel::~grid_panel() {
	}

	void grid_panel::on_child_added(window* child) {
		auto child_component = child->as_component();
		IF_VERIFY(child_component != nullptr) {
			child_component->set_will_self_update_area(false);
			child_component->set_has_grid_position(true);
		}
	}

	void grid_panel::on_area_changed() {
		auto update_row_area_func = [&](row_column& rc, int rc_offset, int rc_size) {
			rc._area = rect(get_area().get_top_left() + point(0, rc_offset), size(get_area().get_width(), rc_size));
		};
		update_row_column_areas(_rows, get_area().get_height(), update_row_area_func);

		auto update_column_area_func = [&](row_column& rc, int rc_offset, int rc_size) {
			rc._area = rect(get_area().get_top_left() + point(rc_offset, 0), size(rc_size, get_area().get_height()));
		};
		update_row_column_areas(_columns, get_area().get_width(), update_column_area_func);

		for (auto& child : get_children()) {
			if (check_child_grid_position(*child)) {
				const auto& grid_position = child->as_component()->get_grid_position();

				int top = get_area().get_top();
				int bottom = get_area().get_bottom();
				if (grid_position._row.has_value()) {
					top = _rows.at(grid_position._row.get_value())._area.get_top();
					bottom = _rows.at(grid_position._row.get_value())._area.get_bottom();
				}

				int left = get_area().get_left();
				int right = get_area().get_right();
				if (grid_position._column.has_value()) {
					left = _columns.at(grid_position._column.get_value())._area.get_left();
					right = _columns.at(grid_position._column.get_value())._area.get_right();
				}

				child->as_component()->update_area(rect(point(left, top), point(right, bottom)));
			}
		}
	}

	bool grid_panel::check_child_grid_position(window& child) const {
		bool is_valid = true;

		const auto& grid_position = child.as_component()->get_grid_position();

		if (grid_position._row.has_value()) {
			if (grid_position._row.get_value() < 0 || grid_position._row.get_value() >= static_cast<int>(_rows.size())) {
				is_valid = false;
			}
		}

		if (grid_position._column.has_value()) {
			if (grid_position._column.get_value() < 0 || grid_position._column.get_value() >= static_cast<int>(_columns.size())) {
				is_valid = false;
			}
		}
			
		if (!is_valid) {
			ALERT("child grid position is out of grid range\n\nchild : {}\ngrid_size : {} x {}\ngrid_position : {} x {}",
				child.get_id(),
				_columns.size(), _rows.size(),
				grid_position._column.get_value_or(-1),
				grid_position._row.get_value_or(-1));
		}

		return is_valid;
	}

	void grid_panel::update_row_column_areas(row_column_vector& row_columns, int total_size, std::function<void(row_column& rc, int offset, int size)> update_rc_func) {
		int no_size_size = 0;
		int no_size_count = count_if(row_columns, [](const row_column& rc) { return !rc._size.has_value(); });
		if (no_size_count > 0) {
			int used_size = accumulate(row_columns, 0, [](int s, const row_column& rc) { return s + rc._size.get_value_or(0); });
			int total_size_unscaled = float_to_int(total_size / get_area_scale()); //will be scaled again next loop
			int available_size = total_size_unscaled - used_size;
			if (available_size > 0) {
				no_size_size = available_size / no_size_count;
			}
		}

		int current_offset = 0;
		for (unsigned int rc_index = 0; rc_index < row_columns.size(); rc_index++) {
			const auto& rc = row_columns.at(rc_index);
			int rc_size = rc._size.get_value_or(no_size_size);
			int rc_size_scaled = float_to_int(rc_size * get_area_scale());
			int available_size = total_size - current_offset;
			int rc_size_final = std::min(rc_size_scaled, available_size);

			update_rc_func(row_columns.at(rc_index), current_offset, rc_size_final);
			current_offset += rc_size_final;
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


