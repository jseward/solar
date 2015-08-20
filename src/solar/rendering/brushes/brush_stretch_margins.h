#pragma once

#include "solar/archiving/single_value_archivable.h"
#include "solar/math/rect.h"
#include "solar/containers/enum_vector.h"
#include "brush_stretch_region.h"

namespace solar {

	class brush_stretch_margins : public single_value_archivable {
	private:
		int _left;
		int _top;
		int _right;
		int _bottom;

	public:
		brush_stretch_margins();
		brush_stretch_margins(int left, int top, int right, int bottom);

		bool has_any() const;
		bool has_left() const;
		bool has_top() const;
		bool has_right() const;
		bool has_bottom() const;

		int get_left() const;
		int get_top() const;
		int get_right() const;
		int get_bottom() const;

		enum_vector<brush_stretch_region, brush_stretch_region> get_all_non_empty_stretch_regions() const;

		bool can_build_region_rect(const rect& full_rect) const;
		rect build_region_rect(const rect& full_rect, brush_stretch_region region) const;

	private:
		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;

	public:
		friend std::ostream& operator<<(std::ostream& os, const brush_stretch_margins& margins);
		friend brush_stretch_margins make_scaled_brush_stretch_margins(const brush_stretch_margins& unscaled_margins, float scale);
	};

}
