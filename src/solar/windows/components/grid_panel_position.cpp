#include "grid_panel_position.h"

#include "solar/archiving/archiving_helpers.h"

namespace solar {

	grid_panel_position::grid_panel_position() 
		: _row()
		, _column() {
	}

	void grid_panel_position::read_from_archive(archive_reader& reader) {
		read_optional_int(reader, "has_row", "row", _row);
		read_optional_int(reader, "has_column", "column", _column);
	}

	void grid_panel_position::write_to_archive(archive_writer& writer) const {
		write_optional_int(writer, "has_row", "row", _row);
		write_optional_int(writer, "has_column", "column", _column);
	}

}
