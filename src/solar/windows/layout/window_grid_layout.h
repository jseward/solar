#pragma once

#include "solar/math/rect.h"
#include "solar/math/size.h"
#include "solar/archiving/archivable.h"
#include "window_layout.h"

namespace solar {

	class window;

	class window_grid_layout : public archivable {
	private:
		window_layout _layout;
		size _cell_stride;
		size _cell_size;

	public:
		rect build_cell_area(const rect& parent_area, float scale, int row, int column) const;
		rect build_cell_area(const window& parent_window, int row, int column) const;

	private:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}