#include "prim2d.h"
#include "solar/math/point_helpers.h"

namespace solar {

	void prim2d::render_rect(const rect& area, const color& color) {
		render_rect(
			point_to_vec2(area.get_top_left()),
			point_to_vec2(area.get_top_right()),
			point_to_vec2(area.get_bottom_right()),
			point_to_vec2(area.get_bottom_left()),
			color,
			simple_rect_uvs());
	}

	void prim2d::render_rect(const rect& area, const color& color, const simple_rect_uvs& uvs) {
		render_rect(
			point_to_vec2(area.get_top_left()),
			point_to_vec2(area.get_top_right()),
			point_to_vec2(area.get_bottom_right()),
			point_to_vec2(area.get_bottom_left()),
			color,
			uvs);
	}

}