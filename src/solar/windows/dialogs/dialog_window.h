#pragma once

#include "solar/windows/window.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/text/text_id.h"
#include "dialog_window_style.h"
#include "dialog_frame_window.h"
#include "dialog_content_window.h"
#include "dialog_title_window.h"
#include "dialog_close_button.h"

namespace solar {

	class dialog_window : public window {
	private:
		dialog_window_style_id _style;
		window_layout _frame_layout;
		text_id _title_text;

		dialog_frame_window _frame_window;
		dialog_content_window _content_window;
		dialog_title_window _title_window;
		dialog_close_button _close_button;

		bool _is_closable_ever;

		bool _is_open;

	public:
		dialog_window(const char* id);
		virtual ~dialog_window();

		void set_is_closable_ever(bool is_closable_ever);
		bool is_closable_ever() const;
		bool is_closable_now() const;

		const dialog_window_style& get_style() const;
		const window_layout& get_frame_layout() const;
		const wchar_t* get_title_text() const;

		void add_child_to_content_window(window* child);
		void remove_all_children_from_content_window();

		void open();
		void close();
		bool try_close();

		virtual void on_parent_area_changed() override;
		virtual bool on_key_down_anywhere(const window_key_event_params& params) override;
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params) override;
		virtual bool on_mouse_button_up(const window_mouse_button_event_params& params) override;
		virtual void render(const window_render_params& params) override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

		virtual void on_opened();
		virtual void on_closed();
	};

}