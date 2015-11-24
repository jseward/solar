#pragma once

#include "solar/archiving/single_value_archivable.h"
#include "solar/math/rect.h"

namespace solar {

	class window_layout_margins : public single_value_archivable {
	private:
		int _left;
		int _right;
		int _top;
		int _bottom;

	public:
		window_layout_margins();
		window_layout_margins(int left, int right, int top, int bottom);

		int get_left() const;
		int get_right() const;
		int get_top() const;
		int get_bottom() const;

		window_layout_margins operator*(float rhs) const;

		rect transform_area(const rect& in_area, float in_area_scale) const;

	private:
		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;
	};

}