#pragma once

#include "solar/windows/window_component.h"
#include "solar/math/rectf.h"
#include "text_entry_style.h"

namespace solar {

	class text_entry : public window_component {
	public:
		using text_changed_callback_fucntion = std::function<void(const text_entry&)>;
		using apply_changes_callback_function = std::function<void(const text_entry&)>;
		using char_added_callback_function = std::function<void(const text_entry&, wchar_t)>;

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
		bool _should_trap_enter;
		text_changed_callback_fucntion _text_changed_callback;
		apply_changes_callback_function _apply_changes_callback;
		char_added_callback_function _char_added_callback;

		std::wstring _text;
		int _visible_pos;
		int _caret_pos;
		int _selection_pos;
		bool _is_dragging_selection;
		bool _has_changes_to_apply;

		mutable render_text_parts _render_text_parts;

	public:
		text_entry(const char* id);
		virtual ~text_entry();

		void set_should_trap_enter(bool should_trap);
		void set_text_changed_callback(text_changed_callback_fucntion callback);
		void set_apply_changes_callback(apply_changes_callback_function callback);
		void set_char_added_callback(char_added_callback_function callback);

		const wchar_t* get_text() const;
		void set_text_and_ignore_change(const wchar_t* new_text);

	protected:
		virtual bool is_focusable_ever() const override;
		virtual void render(const window_render_params& params) override;
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params) override;
		virtual void on_focus_gained() override;
		virtual void on_focus_lost(bool should_apply_changes) override;
		virtual bool on_char_received(const window_char_event_params& params) override;
		virtual bool on_key_down(const window_key_event_params& params) override;

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void refresh_render_text_parts();
		void refresh_render_text_part(render_text_part& part, const render_text_part* previous_part, int begin_pos, int end_pos, const rectf& available_area);
		rect get_text_area() const;
		float get_text_width(const std::wstring& text) const;
		float get_kerning_pair_offset(wchar_t a, wchar_t b) const;
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
		const font& get_font() const;
		float get_font_size() const;
		void try_apply_changes();
		void try_add_char(wchar_t c);
		void handle_text_changed();
		void handle_char_added(wchar_t c);
		void handle_backspace_key_down();
		void handle_delete_key_down();
		bool is_char_valid(wchar_t c) const;
		void move_caret_left(int count, bool should_select);
		void move_caret_left_to_next_word(bool should_select);
		void move_caret_right(int count, bool should_select);
		void move_caret_right_to_next_word(bool should_select);
		void move_caret_home(bool should_select);
		void move_caret_end(bool should_select);
		bool is_pos_word_break(int pos) const;
		void try_delete_selection();
		void copy_text_to_clipboard();
		void cut_text_to_clipboard();
		void paste_text_from_clipboard();
	};

}
