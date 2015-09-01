#pragma once

#include "solar/windows/window_component.h"

namespace solar {

	class grid_panel_row : public archivable {
	public:
		int _height;

		mutable rect _current_area;

	public:
		grid_panel_row();
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	class grid_panel_column : public archivable {
	public:
		int _width;

		mutable rect _current_area;

	public:
		grid_panel_column();
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	class grid_panel : public window_component {
	private:
		std::vector<grid_panel_row> _rows;
		std::vector<grid_panel_column> _columns;

	public:
		grid_panel(const char* id);
		virtual ~grid_panel();

	protected:
		virtual void on_child_added(window* child) override;
		virtual void on_area_changed() override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}