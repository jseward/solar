#include "brush_renderer.h"
#include "brush.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/containers/container_helpers.h"
#include "solar/utility/unused_parameter.h"
#include "solar/rendering/primatives/prim2d.h"

namespace solar {

	brush_renderer::brush_renderer(prim2d& prim2d) 
		: _prim2d(prim2d) {
	}

	brush_renderer::~brush_renderer() {
	}

	void brush_renderer::begin_rendering(const rect& render_area, shader& shader, render_state_block* rs_block) {
		_prim2d.begin_rendering(render_area, shader, rs_block);
	}

	void brush_renderer::end_rendering() {
		_prim2d.end_rendering();
	}

	void brush_renderer::render_brush(
		const brush& brush, 
		brush_render_mode render_mode, 
		const rect& area, 
		float area_scale, 
		const color& color) {

		switch (render_mode) {
			case brush_render_mode::STRETCHED:
				render_brush_stretched(brush, area, area_scale, color);
				break;

			case brush_render_mode::TILED:
				render_brush_tiled(brush, area, area_scale, color);
				break;

			case brush_render_mode::invalid:
			case brush_render_mode::count:
			default: ASSERT(false); break;
		}
	}

	void brush_renderer::render_brush_stretched(const brush& brush, const rect& area, float area_scale, const color& color) {
		_prim2d.set_texture(brush.get_texture());

		auto margins = brush.get_stretch_margins();
		auto regions = margins.get_all_non_empty_stretch_regions();
		if (regions.empty()) {
			_prim2d.render_rect(area, color, brush.get_uvs(brush_stretch_region::CENTER));
		}
		else {
			auto scaled_margins = make_scaled_brush_stretch_margins(margins, area_scale);
			if (!scaled_margins.can_build_region_rect(area)) {
				ALERT("brush cannot be rendered as stretch margins do not fit within the area given.\n\nbrush : {}\narea : {}\nstretch_margins: {}", brush, area, scaled_margins);
			}
			else {
				for (auto region : regions) {
					_prim2d.render_rect(scaled_margins.build_region_rect(area, region), color, brush.get_uvs(region));
				}
			}
		}
	}

	void brush_renderer::render_brush_with_angle(const brush& brush, const rect& area, const color& color, float angle) {
		_prim2d.set_texture(brush.get_texture());

		if (brush.get_stretch_margins().has_any()) {
			static bool has_alerted_warning = false;
			if (!has_alerted_warning) {
				ALERT("WARNING : brushes with stretch margines not supported when rendering with angle. brush:{}", brush.get_id());
				has_alerted_warning = true;
			}
		}

		_prim2d.render_rect_with_angle(area, color, brush.get_uvs(brush_stretch_region::CENTER), angle);
	}

	void brush_renderer::render_brush_tiled(const brush& brush, const rect& area, float area_scale, const color& color) {
		UNUSED_PARAMETER(color);
		UNUSED_PARAMETER(area_scale);
		UNUSED_PARAMETER(area);
		UNUSED_PARAMETER(brush);
		ASSERT(false);
	}

}