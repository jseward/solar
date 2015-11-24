#include "opengl_prim2d_lines.h"

#include "solar/utility/unused_parameter.h"

namespace solar {

	opengl_prim2d_lines::opengl_prim2d_lines()
		: _is_rendering(false) {
	}

	opengl_prim2d_lines::~opengl_prim2d_lines() {
		ASSERT(!_is_rendering);
	}

	void opengl_prim2d_lines::setup() {
	}

	void opengl_prim2d_lines::teardown() {
	}

	bool opengl_prim2d_lines::is_rendering() const {
		return _is_rendering;
	}

	void opengl_prim2d_lines::begin_rendering(float line_width) {
		ASSERT(!_is_rendering);
		_is_rendering = true;

		//todo
		UNUSED_PARAMETER(line_width);
	}

	void opengl_prim2d_lines::end_rendering() {
		ASSERT(_is_rendering);
		_is_rendering = false;

		//todo
	}

	void opengl_prim2d_lines::render_segments(const vec2* points, unsigned int point_count, const color& color) {
		ASSERT(_is_rendering);

		//todo
		UNUSED_PARAMETER(points);
		UNUSED_PARAMETER(point_count);
		UNUSED_PARAMETER(color);
	}

}
