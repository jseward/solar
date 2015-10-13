#pragma once

#include "solar/windows/window_component.h"

namespace solar {

	enum class stack_panel_orientation {
		HORIZONTAL,
		VERTICAL
	};

	class stack_panel : public window_component {
	private:
		const stack_panel_orientation _orientation;

	public:
		stack_panel(const char* id, stack_panel_orientation orientation);
		virtual ~stack_panel();

		virtual void on_area_changed() override;
		virtual void on_child_added(window* child) override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void update_child_areas_horizontal();
		void update_child_areas_vertical();
	};

}
