#include "window_renderer.h"

#include "solar/rendering/brushes/brush_renderer.h"
#include "solar/windows/root_window.h"
#include "solar/windows/window_helpers.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/rendering/brushes/brush_id.h"
#include "solar/rendering/fonts/font_renderer.h"
#include "solar/resources/resource_system.h"

namespace solar {

	window_renderer::window_renderer(resource_system& resource_system, font_renderer& font_renderer, brush_renderer& brush_renderer)
		: _resource_system(resource_system)
		, _font_renderer(font_renderer)
		, _brush_renderer(brush_renderer) {
	}

	window_renderer::~window_renderer() {
	}

	void window_renderer::setup() {
		auto address = _resource_system.resolve_address("window_renderer_def", "windows", ".window_renderer", "_window_renderer", "window_renderer::setup()");
		if (!address.empty()) {
			_resource_system.read_object_as_json(_def, address);
		}
	}

	void window_renderer::teardown() {
	}

	void window_renderer::begin_brush_rendering(window_render_state state) {
		_brush_renderer.begin_rendering(root_window::get().get_area(), _def._brush_shader_ids.at(state));
	}

	void window_renderer::end_brush_rendering() {
		_brush_renderer.end_rendering();
	}

	void window_renderer::render_brush(const window& window, const brush_id& brush_id, brush_render_mode render_mode) {
		if (!brush_id.is_id_empty()) {
			_brush_renderer.render_brush(
				brush_id.get(), 
				render_mode, 
				window.get_area(), 
				window.get_area_scale(), 
				get_default_brush_color(window));
		}
	}

	void window_renderer::render_brush(const window& window, const brush_id& brush_id, brush_render_mode render_mode, const window_layout& layout) {
		if (!brush_id.is_id_empty()) {
			_brush_renderer.render_brush(
				brush_id.get(), 
				render_mode, 
				layout.build_area(window.get_area(), window.get_area_scale()),
				window.get_area_scale(),
				get_default_brush_color(window));
		}
	}

	color window_renderer::get_default_brush_color(const window& window) const {
		return color(1.f, 1.f, 1.f, window.get_alpha());
	}

	void window_renderer::begin_font_rendering() {
		_font_renderer.begin_rendering(root_window::get().get_area());
	}

	void window_renderer::end_font_rendering() {
		_font_renderer.end_rendering();
	}

	void window_renderer::render_font(const window& window, const window_font_def& font_def, const wchar_t* text) {
		float font_size = font_def._font_size * window.get_font_scale();
		_font_renderer.render(font_render_params(font_def._font_id.get(), font_size, text)
			.set_alignment(font_def._alignment)
			.set_is_multiline(font_def._is_multiline)
			.set_is_clipping_enabled(font_def._is_clipping_enabled)
			.set_render_area(window.get_area())
			.set_color(make_window_alpha_scaled_color(window, font_def._color))
			.set_is_dropshadow_enabled(font_def._is_dropshadow_enabled)
			.set_dropshadow_def(font_def._dropshadow_def));
	}

}