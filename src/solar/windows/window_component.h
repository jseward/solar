#pragma once

#include "window.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/windows/components/grid_panel_position.h"

namespace solar {

	//window class was getting a bit heavyweight. adding another layer of hierarchy for things that all window components (buttons, etc) will require. but not essential for base window class.

	class window_component : public window {
	private:
		bool _has_layout;
		window_layout _layout;
		bool _has_grid_position;
		grid_panel_position _grid_position;
		bool _will_self_update_area;

	public:
		window_component(const char* id);
		virtual ~window_component();

		bool get_has_layout() const;
		void set_has_layout(bool has_layout);
		const window_layout& get_layout() const;

		void set_has_grid_position(bool has_grid_position);
		const grid_panel_position& get_grid_position() const;

		void set_will_self_update_area(bool will_self_update_area);

		void update_area(const rect& parent_area);

	protected:
		virtual void on_loaded() override;
		virtual void on_parent_area_changed() override;
		virtual window_component* as_component() override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	protected:
		void try_update_area();
	};

}