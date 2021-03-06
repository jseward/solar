#include "window_renderer.h"

#include "solar/rendering/brushes/brush_renderer.h"
#include "solar/windows/root_window.h"
#include "solar/windows/window_helpers.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/rendering/brushes/brush_id.h"
#include "solar/rendering/fonts/font_renderer.h"
#include "solar/resources/resource_system.h"

namespace solar {

	window_renderer::window_renderer(render_device& render_device, resource_system& resource_system, prim2d& prim2d, prim2d_lines& prim2d_lines, font_renderer& font_renderer, brush_renderer& brush_renderer)
		: _resource_system(resource_system)
		, _prim2d(prim2d)
		, _prim2d_lines(prim2d_lines)
		, _font_renderer(font_renderer)
		, _brush_renderer(brush_renderer) 
		, _render_device(render_device) {
	}

	window_renderer::~window_renderer() {
	}

	void window_renderer::setup() {
		auto address = _resource_system.resolve_address_to_file("window_renderer_def", "windows", ".window_renderer", "_window_renderer", "window_renderer::setup()");
		if (!address.empty()) {
			_resource_system.read_object_as_json(_def, address);
		}

		_render_state_block = make_render_state_block_ptr(_render_device, render_state_block_def()
			.set_depth_write(render_state_depth_write::DISABLED)
			.set_depth_compare_func(render_state_compare_func::NONE)
			.set_blend(render_state_blend_type::SRC_ALPHA, render_state_blend_type::INV_SRC_ALPHA));
	}

	void window_renderer::teardown() {
		_render_state_block.reset();
	}

	prim2d& window_renderer::get_prim2d() {
		return _prim2d;
	}

	prim2d_lines& window_renderer::get_prim2d_lines() {
		return _prim2d_lines;
	}

	brush_renderer& window_renderer::get_brush_renderer() {
		return _brush_renderer;
	}

	void window_renderer::begin_brush_rendering() {
		_brush_renderer.begin_rendering(root_window::get().get_area());
	}

	void window_renderer::end_brush_rendering() {
		_brush_renderer.end_rendering();
	}

	void window_renderer::render_brush(const window& window, const brush* brush) {
		render_brush(window, brush, brush_render_mode::STRETCHED);
	}

	void window_renderer::render_brush(const window& window, const brush* brush, brush_render_mode render_mode) {
		if (brush != nullptr && !brush->is_id_empty()) {
			_brush_renderer.render_brush(
				*brush,
				render_mode, 
				window.get_area(), 
				window.get_area_scale(), 
				get_default_brush_color(window));
		}
	}

	void window_renderer::render_brush(const window& window, const brush* brush, brush_render_mode render_mode, const window_layout& layout) {
		if (brush != nullptr  && !brush->is_id_empty()) {
			_brush_renderer.render_brush(
				*brush,
				render_mode, 
				layout.build_area(window.get_area(), window.get_area_scale()),
				window.get_area_scale(),
				get_default_brush_color(window));
		}
	}

	void window_renderer::render_brush(const window& window, const brush* brush, brush_render_mode render_mode, const rect& render_area) {
		if (brush != nullptr  && !brush->is_id_empty()) {
			_brush_renderer.render_brush(
				*brush,
				render_mode,
				render_area,
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
		render_font(window, font_def, text, window.get_area());
	}

	void window_renderer::render_font(const window& window, const window_font_def& font_def, const wchar_t* text, const rect& render_area) {
		float font_size = font_def.get_scaled_font_size(window.get_font_scale());
		_font_renderer.render(font_render_params(font_def._font_id.get(), font_size, text)
			.set_alignment(font_def._alignment)
			.set_is_multiline(font_def._is_multiline)
			.set_is_clipping_enabled(font_def._is_clipping_enabled)
			.set_render_area(render_area)
			.set_color(make_window_alpha_scaled_color(window, font_def._color))
			.set_is_dropshadow_enabled(font_def._is_dropshadow_enabled)
			.set_dropshadow_def(font_def._dropshadow_def));
	}

}