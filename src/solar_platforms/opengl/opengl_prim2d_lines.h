#pragma once

#include "solar/rendering/primatives/prim2d_lines.h"

namespace solar {

	class opengl_prim2d_lines : public prim2d_lines {
	private:
		bool _is_rendering;

	public:
		opengl_prim2d_lines();
		virtual ~opengl_prim2d_lines();
		
		void setup();
		void teardown();

		virtual bool is_rendering() const override;
		virtual void begin_rendering(float line_width) override;
		virtual void end_rendering() override;
		virtual void render_segments(const vec2* points, unsigned int point_count, const color& color) override;
	};

}