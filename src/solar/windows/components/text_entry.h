#pragma once

#include "solar/windows/window_component.h"
#include "solar/math/rectf.h"
#include "text_entry_style.h"

namespace solar {

	class text_entry : public window_component {
	private:
		struct render_text_part {
			std::wstring _unclipped_text;
			std::wstring _clipped_text;
			rectf _area;
		};

		struct render_text_parts {
		public:
			render_text_part _pre_selection;
			render_text_part _selection;
			render_text_part _post_selection;
		};

	private:
		text_entry_style_id _style;

		unsigned int _max_text_length;
		bool _is_password;
		bool _is_read_only;

		std::wstring _text;
		int _visible_pos;
		int _caret_pos;
		int _selection_pos;
		bool _is_dragging_selection;

		mutable render_text_parts _render_text_parts;

	public:
		text_entry(const char* id);
		virtual ~text_entry();

		void set_text_ignore_change(const wchar_t* new_text);

	protected:
		virtual bool is_focusable_ever() const override;
		virtual void render(const window_render_params& params) override;
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) override;

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void refresh_render_text_parts();
		void refresh_render_text_part(render_text_part& part, const render_text_part* previous_part, int begin_pos, int end_pos, const rectf& available_area);
		rect get_text_area() const;
		float get_text_width(const std::wstring& text) const;
		float get_kerning_width(wchar_t a, wchar_t b) const;
		int get_selection_begin() const;
		int get_selection_end() const;
		int get_selection_length() const;
		int get_caret_offset_from_left() const;
		rect get_caret_area() const;
		color get_caret_color() const;
		void copy_text_to_fit(const wchar_t* new_text);
		void make_caret_visible();
		int get_cursor_caret_pos(const point& cursor_pos) const;
		void raw_text_to_visible_text(std::wstring& s) const;
	};

}
