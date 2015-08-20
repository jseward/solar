#pragma once

#include <vector>
#include "font_renderer_line.h"
#include "font_render_params.h"

namespace solar {

	class font_renderer_line_builder {
	private:
		std::vector<font_renderer_line> _lines;

	public:
		font_renderer_line_builder();

		void build_lines(const font_render_params& params);

		const std::vector<font_renderer_line>& get_lines() const;

	private:
		void build_lines_singleline(const font_render_params& params);
		void build_lines_multiline(const font_render_params& params);
		bool is_word_break_char(wchar_t c) const;
	};

}