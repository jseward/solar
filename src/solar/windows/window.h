#pragma once

#include <vector>
#include <memory>
#include <functional>
#include "solar/math/rect.h"
#include "solar/archiving/archivable.h"
#include "./rendering/window_render_params.h"
#include "./event_params/window_mouse_button_event_params.h"
#include "./event_params/window_mouse_wheel_event_params.h"
#include "./event_params/window_key_event_params.h"
#include "./event_params/window_char_event_params.h"
#include "window_focus_controller.h"

namespace solar {

	class window_component;

	class window : public archivable {
	public:
		using is_visible_callback_function = std::function<bool()>;
		using is_enabled_callback_function = std::function<bool()>;

	private:
		const char* _id;
		rect _area;
		float _alpha;
		bool _is_visible;
		is_visible_callback_function _is_visible_callback;
		is_enabled_callback_function _is_enabled_callback;
		bool _can_be_under_cursor;
		bool _will_clip_viewport;
		
		window* _parent;
		window_focus_controller* _parent_focus_controller;
		bool _are_children_locked;
		std::vector<window*> _children;
		std::unique_ptr<window_focus_controller> _this_focus_controller;

	public:
		window(const char* id);
		window(const window&) = delete;
		window& operator=(const window&) = delete;
		virtual ~window();
		
		void set_is_visible_callback(is_visible_callback_function is_visible_callback);
		void set_is_enabled_callback(is_enabled_callback_function is_enabled_callback);

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

		float get_alpha() const;

		bool is_enabled() const;

		void set_is_visible(bool is_visible);
		bool is_visible() const;
		bool is_visible_recursive() const;

		bool can_be_under_cursor() const;

		bool will_clip_viewport() const;

		void set_is_focus_controller(window_focus_controller_should_handle_key_down should_handle_key_down);
		window_focus_controller* as_focus_controller();
		void try_make_focused();
		bool is_focusable_now() const;
		bool is_focused() const;

	public:
		virtual void render(const window_render_params& params);
		virtual void render_after_children(const window_render_params& params);

	public: //NOTE: should be protected but they can't be used in lamba expressions (ex. root_window::root_on_mouse_button_down)
		virtual void on_loaded();
		virtual void on_parent_area_changed();
		virtual void on_area_changed();
		virtual void on_child_added(window* child);
		virtual void on_child_removed(window* child);
		virtual bool on_mouse_button_down(const window_mouse_button_event_params& params);
		virtual bool on_mouse_button_up(const window_mouse_button_event_params& params);
		virtual bool on_mouse_button_up_anywhere(const window_mouse_button_event_params& params);
		virtual bool on_mouse_wheel_moved(const window_mouse_wheel_event_params& params);
		virtual bool on_key_down(const window_key_event_params& params);
		virtual bool on_key_down_anywhere(const window_key_event_params& params);
		virtual bool on_char_received(const window_char_event_params& params);
		virtual void on_focus_lost(bool should_apply_changes);
		virtual void on_focus_gained();
		virtual bool is_focusable_ever() const;
		virtual void on_is_visible_changed();
		virtual float get_area_scale() const;
		virtual float get_font_scale() const;

		//cheap replacement for dynamic cast. needed so on_child_added() can useful actions like querying layout information.
		virtual window_component* as_component();

	protected:
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;

	private:
		void try_attach_self_and_children_to_best_focus_controller();
		void try_detach_self_and_children_from_focus_controller();
	};

}