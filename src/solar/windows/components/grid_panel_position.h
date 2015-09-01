#pragma once

#include "solar/archiving/archivable.h"
#include "solar/utility/optional.h"

namespace solar {

	class grid_panel_position : public archivable {
	public:
		optional<int> _row;
		optional<int> _column;

	public:
		grid_panel_position();
		
		virtual void read_from_archive(archive_reader& reader);
		virtual void write_to_archive(archive_writer& writer) const;
	};

}