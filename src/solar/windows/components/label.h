#pragma once

#include "solar/windows/window_component.h"
#include "label_style.h"

namespace solar {

	class label : public window_component {
	private:
		label_style_id _style;

	public:
		label(const char* id);
		virtual ~label();

		virtual void render(const window_render_params& params) override;

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}