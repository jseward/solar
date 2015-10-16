#pragma once

#include "solar/windows/window_component.h"
#include "solar/text/text_id.h"
#include "label_style.h"

namespace solar {

	class label : public window_component {
	public:
		using get_text_callback_function = std::function<const wchar_t*()>;

	private:
		label_style_id _style;
		get_text_callback_function _get_text_callback;
		text_id _text;

	public:
		label(const char* id);
		virtual ~label();

		label& set_get_text_callback(get_text_callback_function get_text_callback);

		virtual void render(const window_render_params& params) override;

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		const wchar_t* get_text() const;
	};

}