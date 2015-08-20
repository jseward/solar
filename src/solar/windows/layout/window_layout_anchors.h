#pragma once

#include "solar/archiving/single_value_archivable.h"

namespace solar {

	class window_layout_anchors : public single_value_archivable {
	private:
		bool _left;
		bool _right;
		bool _top;
		bool _bottom;

	public:
		window_layout_anchors();

		bool has_left() const;
		bool has_right() const;
		bool has_top() const;
		bool has_bottom() const;

	private:
		virtual void read_from_archive(archive_reader& reader, const char* name) override;
		virtual void write_to_archive(archive_writer& writer, const char* name) const override;
	};

}