#include "font.h"
#include "solar/resources/resource_system.h"
#include "solar/utility/verify.h"
#include "solar/utility/type_convert.h"
#include "bm_font_reader.h"

namespace solar {

	font::font(const resource_address& address)
		: _address(address)
		, _font_height(0)
		, _line_height(0)
		, _line_top_to_base_distance(0) {
	}

	const resource_address& font::get_address() const {
		return _address;
	}

	void font::load(resource_system& resource_system) {
		auto stream = resource_system.open_stream_to_read(_address);
		if (stream != nullptr) {
			bm_font_reader reader(*stream);
			reader.read_font(*this);
			update_page_texture_pixel_size();
			resource_system.close_stream(stream);
		}
	}

	void font::create_textures(texture_factory& texture_factory, resource_system& resource_system) {
		for (auto& page_texture_id : _page_texture_ids) {
			page_texture_id.create_texture(texture_factory, resource_system, texture_create_params().set_has_mip_maps(false));
		}
	}

	void font::release_textures(texture_factory& texture_factory) {
		for (auto& page_texture_id : _page_texture_ids) {
			page_texture_id.release_texture(texture_factory);
		}
	}

	float font::get_line_height(float font_size) const {
		return (_line_height * get_scale(font_size));
	}

	float font::get_char_width(float font_size, wchar_t c) const {
		auto glyph = find_best_glyph(c);
		if (glyph != nullptr) {
			return glyph->_x_stride * get_scale(font_size);
		}
		return 0.f;
	}

	float font::get_text_width(float font_size, const wchar_t* text) const {
		return get_text_width(font_size, text, 0, get_string_length(text));
	}

	float font::get_text_width(float font_size, const wchar_t* text, unsigned int begin_index, unsigned int end_index) const {
		float scale = get_scale(font_size);
		
		float width = 0.f;
		for (unsigned int index = begin_index; index < end_index; ++index) {
			auto glyph = find_best_glyph(text[index]);
			if (glyph != nullptr) {
				width += (glyph->_x_stride * scale);
				
				unsigned int next_index = index + 1;
				if (next_index < end_index) {
					width += (glyph->get_kerning_offset(text[next_index]) * scale);
				}
			}
		}
		return width;
	}

	float font::get_kerning_pair_offset(float font_size, wchar_t a, wchar_t b) const {
		auto glyph = find_best_glyph(a);
		if (glyph != nullptr) {
			return glyph->get_kerning_offset(b) * get_scale(font_size);
		}
		return 0.f;
	}

	float font::get_scale(float font_size) const {
		IF_VERIFY(_font_height > 0) {
			return font_size / _font_height;
		}
		return 1.f;
	}

	const font_glyph* font::find_best_glyph(wchar_t c) const {
		auto iter = _glyph_map.find(c);
		if (iter != _glyph_map.end()) {
			return &iter->second;
		}

		if (c != L'\n' && c != L'\r') {
			auto fallback_iter = _glyph_map.find(L'?');
			IF_VERIFY(fallback_iter != _glyph_map.end()) {
				return &fallback_iter->second;
			}
		}

		return nullptr;
	}

	texture* font::get_page_texture(int page) const {
		return _page_texture_ids[page].get_texture();
	}

	sizef font::get_page_texture_pixel_size() const {
		return _page_texture_pixel_size;
	}


	void font::update_page_texture_pixel_size() {
		IF_VERIFY(_page_size._width > 0 && _page_size._height > 0) {
			_page_texture_pixel_size = sizef(
				1.f / int_to_float(_page_size._width),
				1.f / int_to_float(_page_size._height));
		}
	}

}