#pragma once

#include <vector>
#include <memory>
#include <functional>
#include "solar/math/rect.h"
#include "solar/archiving/archivable.h"
#include "window_render_params.h"
#include "window_mouse_button_event_params.h"

namespace solar {

	class window_component;

	class window : public archivable {
	public:
		using is_visible_callback_function = std::function<bool()>;

	private:
		const char* _id;
		rect _area;
		float _alpha;
		bool _is_visible;
		is_visible_callback_function _is_visible_callback;
		bool _can_be_under_cursor;
		bool _will_clip_viewport;
		
		window* _parent;
		bool _are_children_locked;
		std::vector<window*> _children;
		
	public:
		window(const char* id);
		window(const window&) = delete;
		window& operator=(const window&) = delete;
		virtual ~window();
		
		void set_is_visible_callback(is_visible_callback_function is_visible_callback);

		const char* get_id() const;

		void lock_children();
		void unlock_children();
		void add_child(window* child);
		void remove_child(window* child);
		void remove_all_children();
		const std::vector<window*>& get_children() const;
		bool has_parent() const;
		window& get_parent() const;

		void set_area(const rect& new_area);
		const rect& get_area() const;
		
		float get_area_scale() const;		
		float get_font_scale() const;

		float get_alpha() const;

		bool is_enabled() const;

		bool is_focused() const;

		void set_is_visible(bool is_visible);
		bool is_visible() const;
		bool can_be_under_cursor() const;

		bool will_clip_viewport() const;

	public:
		virtual void render(const window_render_params& params);
		virtual void render_after_children(const window_render_params& params);

	public: //NOTE: should be protected but they can't be used in lamba expressions (ex. root_window::root_on_mouse_button_down)
		virtual void on_loaded();
		virtual void on_parent_area_changed();
		virtual void on_area_changed();
		virtual void on_child_added(window* child);
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params);
		virtual bool on_mouse_button_up(const window_mouse_button_event_params& params);
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params);
		virtual window_component* as_component(); //cheap replacement for dynamic cast. needed so on_child_added() can useful actions like querying layout information.

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

}