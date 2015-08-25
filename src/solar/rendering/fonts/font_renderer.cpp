#include "font_renderer.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/resources/resource_system.h"
#include "solar/rendering/primatives/prim2d.h"
#include "solar/strings/string_marshal.h"
#include "solar/math/rectf.h"

namespace solar {

	namespace font_renderer_impl {
		namespace shader_param_names {
			static const char* FONT_SCALE = "font_scale";
			static const char* TEXTURE_PIXEL_WIDTH = "texture_pixel_width";
			static const char* TEXTURE_PIXEL_HEIGHT = "texture_pixel_height";
			static const char* DROPSHADOW_OFFSET = "dropshadow_offset";
			static const char* DROPSHADOW_MIN_DISTANCE = "dropshadow_min_distance";
			static const char* DROPSHADOW_MAX_DISTANCE = "dropshadow_max_distance";
		}
	}

	font_renderer::font_renderer(resource_system& resource_system, prim2d& prim2d)
		: _resource_system(resource_system)
		, _prim2d(prim2d) {
	}

	font_renderer::~font_renderer() {
	}

	void font_renderer::setup() {
		auto address = _resource_system.resolve_address("font_renderer_def", "fonts", ".font_renderer", "_font_renderer", "font_renderer::setup()");
		if (!address.empty()) {
			_resource_system.read_object_as_json(_def, address);
		}
	}

	void font_renderer::teardown() {
	}

	void font_renderer::begin_rendering(const rect& viewport_area) {
		_prim2d.begin_rendering(viewport_area, _def._normal_shader_id.get());
	}

	void font_renderer::end_rendering() {
		_prim2d.end_rendering();
	}

	void font_renderer::render(const font_render_params& params) {
		_line_builder.build_lines(params);

		if (params._is_dropshadow_enabled) {
			set_dropshadow_shader(params);
			render_lines(params, _line_builder.get_lines(), params._dropshadow_def._color);
		}

		set_normal_shader(params);
		render_lines(params, _line_builder.get_lines(), params._color);
	}

	int font_renderer::calculate_render_height(const font_render_params& params) {
		_line_builder.build_lines(params);
		return float_to_int(_line_builder.get_lines().size() * params._font.get_line_height(params._font_size));
	}

	void font_renderer::render_lines(const font_render_params& params, const std::vector<font_renderer_line>& lines, const color& color) {
		float scale = params._font.get_scale(params._font_size);
		float base_line_error = calculate_base_line_error(params._font, scale);

		for (const auto& line : lines) {
			float x = line._begin_top_left.get_x();
			float y = line._begin_top_left.get_y();

			for (unsigned int index = line._begin_text_index; index < line._end_text_index; ++index) {
				auto glyph = params._font.find_best_glyph(params._text[index]);
				if (glyph != nullptr) {
					float offset_x = (glyph->_offset.get_x() * scale);
					float offset_y = (glyph->_offset.get_y() * scale);
					float width = (glyph->_size.get_width() * scale);
					float height = (glyph->_size.get_height() * scale);
					float right = (x + offset_x + width);
					float base_line = std::roundf(height + offset_y + base_line_error);

					_prim2d.set_texture(params._font.get_page_texture(glyph->_page));
					_prim2d.render_rect(
						rect(
							float_to_int(x + offset_x),
							float_to_int(y + offset_y),
							float_to_int(right),
							float_to_int(y + base_line)),
						color,
						glyph->_uvs);

					x += (glyph->_x_stride * scale);
					
					unsigned int next_index = index + 1;
					if (next_index < line._end_text_index) {
						x += (glyph->get_kerning_offset(params._text[next_index]) * scale);
					}
				}
			}
		}
	}

	float font_renderer::calculate_base_line_error(const font& font, float scale) const {
		auto ref_glyph = font.find_best_glyph(L'a');
		IF_VERIFY(ref_glyph != nullptr) {
			float true_base_line = ref_glyph->_size.get_height() * ref_glyph->_offset.get_y();
			float scaled_base_line = true_base_line * scale;
			return (std::roundf(scaled_base_line) - scaled_base_line);
		}
		return 0.f;
	}

	void font_renderer::set_dropshadow_shader(const font_render_params& params) {
		auto pixel_size = params._font.get_page_texture_pixel_size();
		_def._dropshadow_shader_id->set_float(font_renderer_impl::shader_param_names::TEXTURE_PIXEL_WIDTH, pixel_size.get_width());
		_def._dropshadow_shader_id->set_float(font_renderer_impl::shader_param_names::TEXTURE_PIXEL_HEIGHT, pixel_size.get_height());
		_def._dropshadow_shader_id->set_float_array(font_renderer_impl::shader_param_names::DROPSHADOW_OFFSET, params._dropshadow_def._offset.as_raw_float_array(), 2);
		_def._dropshadow_shader_id->set_float(font_renderer_impl::shader_param_names::DROPSHADOW_MIN_DISTANCE, params._dropshadow_def._min_distance);
		_def._dropshadow_shader_id->set_float(font_renderer_impl::shader_param_names::DROPSHADOW_MAX_DISTANCE, params._dropshadow_def._max_distance);
		_def._dropshadow_shader_id->commit_param_changes();

		_prim2d.set_shader(_def._dropshadow_shader_id.get());
	}

	void font_renderer::set_normal_shader(const font_render_params& params) {
		_def._normal_shader_id->set_float(font_renderer_impl::shader_param_names::FONT_SCALE, params._font.get_scale(params._font_size));
		_def._normal_shader_id->commit_param_changes();

		_prim2d.set_shader(_def._normal_shader_id.get());
	}

}