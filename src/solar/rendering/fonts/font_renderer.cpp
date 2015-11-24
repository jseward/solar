#include "font_renderer.h"
#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "solar/resources/resource_system.h"
#include "solar/rendering/primatives/prim2d.h"
#include "solar/rendering/shaders/shader_program.h"
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

	font_renderer::font_renderer(render_device& render_device, resource_system& resource_system, prim2d& prim2d, font_renderer_shader_program_provider& shader_program_provider)
		: _resource_system(resource_system)
		, _prim2d(prim2d) 
		, _render_device(render_device) 
		, _shader_program_provider(shader_program_provider) {
	}

	font_renderer::~font_renderer() {
	}

	void font_renderer::setup() {
		_render_state_block = make_render_state_block_ptr(_render_device, render_state_block_def()
			.set_depth_write(render_state_depth_write::DISABLED)
			.set_depth_compare_func(render_state_compare_func::NONE)
			.set_blend(render_state_blend_type::SRC_ALPHA, render_state_blend_type::INV_SRC_ALPHA));
	}

	void font_renderer::teardown() {
		_render_state_block.reset();
	}

	void font_renderer::begin_rendering(const rect& viewport_area) {
		_prim2d.begin_rendering(viewport_area, _shader_program_provider.get_normal_shader_program(), _render_state_block.get());
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
			float x = line._begin_top_left._x;
			float y = line._begin_top_left._y;

			for (unsigned int index = line._begin_text_index; index < line._end_text_index; ++index) {
				auto glyph = params._font.find_best_glyph(params._text[index]);
				if (glyph != nullptr) {
					float offset_x = (glyph->_offset._x * scale);
					float offset_y = (glyph->_offset._y * scale);
					float width = (glyph->_size._width * scale);
					float height = (glyph->_size._height * scale);
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
			float true_base_line = ref_glyph->_size._height * ref_glyph->_offset._y;
			float scaled_base_line = true_base_line * scale;
			return (std::roundf(scaled_base_line) - scaled_base_line);
		}
		return 0.f;
	}

	void font_renderer::set_dropshadow_shader(const font_render_params& params) {
		auto& program = _shader_program_provider.get_dropshadow_shader_program();

		auto pixel_size = params._font.get_page_texture_pixel_size();
		program.set_float(font_renderer_impl::shader_param_names::TEXTURE_PIXEL_WIDTH, pixel_size._width);
		program.set_float(font_renderer_impl::shader_param_names::TEXTURE_PIXEL_HEIGHT, pixel_size._height);
		program.set_float_array(font_renderer_impl::shader_param_names::DROPSHADOW_OFFSET, params._dropshadow_def._offset.as_raw_float_array(), 2);
		program.set_float(font_renderer_impl::shader_param_names::DROPSHADOW_MIN_DISTANCE, params._dropshadow_def._min_distance);
		program.set_float(font_renderer_impl::shader_param_names::DROPSHADOW_MAX_DISTANCE, params._dropshadow_def._max_distance);
		program.commit_param_changes();

		_prim2d.set_shader_program(program);
	}

	void font_renderer::set_normal_shader(const font_render_params& params) {
		auto& program = _shader_program_provider.get_normal_shader_program();

		program.set_float(font_renderer_impl::shader_param_names::FONT_SCALE, params._font.get_scale(params._font_size));
		program.commit_param_changes();

		_prim2d.set_shader_program(program);
	}

}