#include "dialog_window.h"

#include "solar/windows/window_renderer.h"
#include "solar/rendering/primatives/prim2d.h"

namespace solar {

	dialog_window::dialog_window(const char* id)
		: window(id)

		, _style(*this)
		, _frame_layout()
		, _title_text()

		, _frame_window(*this)
		, _content_window(*this)
		, _title_window(*this)
		, _close_button(*this)

		, _is_open(false) {

		//using visible callback to prevent extern code being able to call set_is_visible() at any time. need to go through open() and try_close()
		set_is_visible_callback([this]() { return _is_open; });

		add_child(&_frame_window);
		_frame_window.add_child(&_content_window);
		_frame_window.add_child(&_title_window);
		_frame_window.add_child(&_close_button);
	}

	dialog_window::~dialog_window() {
		_frame_window.remove_all_children();
		remove_all_children();
	}

	const dialog_window_style& dialog_window::get_style() const {
		return _style.get();
	}

	const window_layout& dialog_window::get_frame_layout() const {
		return _frame_layout;
	}

	const wchar_t* dialog_window::get_title_text() const {
		return _title_text.get();
	}

	void dialog_window::add_child_to_content_window(window* child) {
		_content_window.add_child(child);
	}

	void dialog_window::remove_all_children_from_content_window() {
		_content_window.remove_all_children();
	}

	void dialog_window::open() {
		_is_open = true;
	}

	bool dialog_window::try_close() {
		ASSERT(false);//todo
		return false;
	}

	void dialog_window::on_parent_area_changed() {
		set_area(get_parent().get_area());
	}

	void dialog_window::render(const window_render_params& params) {
		auto fade_color = _style.get()._parent_fade_color;
		if (fade_color.get_a() > 0.f) {
			params._window_renderer.get_prim2d().begin_rendering(get_area());
			params._window_renderer.get_prim2d().render_rect(get_area(), fade_color);
			params._window_renderer.get_prim2d().end_rendering();
		}
	}

	void dialog_window::read_from_archive(archive_reader& reader) {
		window::read_from_archive(reader);
		read_object(reader, "style", _style);
		read_object(reader, "frame_layout", _frame_layout);
		read_object(reader, "title_text", _title_text);
	}

	void dialog_window::write_to_archive(archive_writer& writer) const {
		window::write_to_archive(writer);
		write_object(writer, "style", _style);
		write_object(writer, "frame_layout", _frame_layout);
		write_object(writer, "title_text", _title_text);
	}

}