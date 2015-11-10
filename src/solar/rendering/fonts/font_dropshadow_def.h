#pragma once

#include "solar/archiving/archivable.h"
#include "solar/math/pointf.h"
#include "solar/colors/color.h"

namespace solar {

	class font_dropshadow_def : public archivable {
	public:
		pointf _offset;
		float _min_distance;
		float _max_distance;
		color _color;

	public:
		font_dropshadow_def();

		void read_from_archive(archive_reader& reader) override;
		void write_to_archive(archive_writer& writer) const override;
	};

}