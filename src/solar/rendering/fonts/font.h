#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "solar/math/sizef.h"
#include "solar/resources/resource_address.h"
#include "solar/rendering/textures/texture_id.h"
#include "font_glyph.h"

namespace solar {

	class resource_system;

	class font {
	private:
		friend class bm_font_reader; //give direct access for convience

	private:
		const resource_address _address;

		std::vector<texture_id> _page_texture_ids;
		std::unordered_map<wchar_t, font_glyph> _glyph_map;
		float _font_height;
		float _line_height;
		int _line_top_to_base_distance;
		size _page_size;
		sizef _page_texture_pixel_size;

	public:
		font(const resource_address& address);

		const resource_address& get_address() const;

		void load(resource_system& resource_system, const char* texture_pool_name);

		float get_scale(float font_size) const;
		float get_line_height(float font_size) const;
		float get_char_width(float font_size, wchar_t c) const;
		float get_text_width(float font_size, const wchar_t* text) const;
		float get_text_width(float font_size, const wchar_t* text, unsigned int begin_index, unsigned int end_index) const;
		float get_kerning_pair_offset(float font_size, wchar_t a, wchar_t b) const;
		const font_glyph* find_best_glyph(wchar_t c) const;
		texture& get_page_texture(int page) const;
		sizef get_page_texture_pixel_size() const;

	private:
		void update_page_texture_pixel_size();
	};

}