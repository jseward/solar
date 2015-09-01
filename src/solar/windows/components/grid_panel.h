#pragma once

#include "solar/windows/window_component.h"

namespace solar {

	class grid_panel : public window_component {
	private:
		class row_column : public archivable {
		public:
			optional<int> _size;
			int _split_distance;
			rect _area;

		public:
			row_column();
			virtual void read_from_archive(archive_reader& reader) override;
			virtual void write_to_archive(archive_writer& writer) const override;
		};

		using row_column_vector = std::vector<row_column>;

	private:
		row_column_vector _rows;
		row_column_vector _columns;

	public:
		grid_panel(const char* id);
		virtual ~grid_panel();

	protected:
		virtual void on_child_added(window* child) override;
		virtual void on_area_changed() override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void update_row_column_areas(row_column_vector& row_columns, int total_size, std::function<void(row_column& rc, int offset, int size)> update_rc_func);
		bool check_child_grid_position(window& child) const;
	};

}