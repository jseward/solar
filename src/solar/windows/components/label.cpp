#include "label.h"

#include "solar/utility/unused_parameter.h"
#include "solar/windows/window_renderer.h"

namespace solar {

	label::label(const char* id)
		: window_component(id)
		, _style(*this) {
	}

	label::~label() {
	}

	void label::set_get_text_callback(get_text_callback_function get_text_callback) {
		_get_text_callback = get_text_callback;
	}

	void label::render(const window_render_params& params) {
		auto& renderer = params._window_renderer;

		renderer.begin_font_rendering();
		renderer.render_font(*this, _style.get()._font_def, get_text());
		renderer.end_font_rendering();
	}

	void label::read_from_archive(archive_reader& reader) {
		window_component::read_from_archive(reader);
		read_object(reader, "style", _style);
		if (_get_text_callback == nullptr) {
			read_object(reader, "text", _text);
		}
	}

	void label::write_to_archive(archive_writer& writer) const {
		window_component::write_to_archive(writer);
		write_object(writer, "style", _style);
		if (_get_text_callback == nullptr) {
			write_object(writer, "text", _text);
		}
	}

	const wchar_t* label::get_text() const {
		if (_get_text_callback != nullptr) {
			return _get_text_callback();
		}
		return _text.get();
	}

}