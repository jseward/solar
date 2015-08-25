#include "window_grid_layout.h"
#include "solar/windows/window.h"
#include "solar/utility/assert.h"
#include "solar/utility/type_convert.h"
#include "solar/archiving/archiving_helpers.h"
#include "solar/archiving/archiving_math_helpers.h"

namespace solar {

	rect window_grid_layout::build_cell_area(const rect& parent_area, float scale, int row, int column) const {
		auto grid_area = _layout.build_area(parent_area, scale);
		point cell_offset = point(
			float_to_int(column * _cell_stride._width * scale),
			float_to_int(row * _cell_stride._height * scale));
		size cell_size = size(
			float_to_int(_cell_size._width * scale),
			float_to_int(_cell_size._height * scale));
		return rect(grid_area.get_top_left() + cell_offset, cell_size);
	}

	rect window_grid_layout::build_cell_area(const window& parent_window, int row, int column) const {
		return build_cell_area(parent_window.get_area(), parent_window.get_area_scale(), row, column);
	}

	void window_grid_layout::read_from_archive(archive_reader& reader) {
		read_object(reader, "layout", _layout);
		read_size(reader, "cell_stride", _cell_stride);
		read_size(reader, "cell_size", _cell_size);
	}

	void window_grid_layout::write_to_archive(archive_writer& writer) const {
		write_object(writer, "layout", _layout);
		write_size(writer, "cell_stride", _cell_stride);
		write_size(writer, "cell_size", _cell_size);
	}

}
