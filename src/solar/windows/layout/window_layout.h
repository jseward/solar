#pragma once

#include "solar/math/rect.h"
#include "solar/math/size.h"
#include "solar/archiving/archivable.h"
#include "window_layout_anchors.h"
#include "window_layout_margins.h"

namespace solar {

	class window;

	class window_layout : public archivable {
	private:
		window_layout_anchors _anchors;
		window_layout_margins _margins;
		size _size;

	public:
		rect build_area(const rect& parent_area, float scale) const;
		rect build_area(const window& parent_window) const;

	private:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}