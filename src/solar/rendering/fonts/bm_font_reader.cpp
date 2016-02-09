#include "bm_font_reader.h"

#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"
#include "solar/math/rect.h"
#include "solar/io/file_path_helpers.h"

namespace solar {
	
	bm_font_reader::bm_font_reader(stream& stream)
		: _stream(stream) {
	}

	void bm_font_reader::read_font(font& font) {
		//based on acgfx_font.cpp

		char magic[5];
		_stream.read_bytes(magic, 4);
		magic[4] = '\0';
		if (!are_strings_equal(magic, "BMF\003")) {
			ALERT("invalid bm_font header : {}", font.get_address());
		}
		else {
			char block_type = 0;
			while (_stream.read_bytes(&block_type, 1) != 0) {
				unsigned int block_size = 0;
				IF_VERIFY(_stream.read_bytes(reinterpret_cast<char*>(&block_size), 4) == 4) {
					auto block_buffer = std::unique_ptr<char[]>(new char[block_size]);
					IF_VERIFY(_stream.read_bytes(block_buffer.get(), block_size) == block_size) {
						switch (block_type) {
						case 1: read_info_block(font, block_buffer.get(), block_size); break;
						case 2: read_common_block(font, block_buffer.get(), block_size); break;
						case 3: read_pages_block(font, block_buffer.get(), block_size); break;
						case 4: read_chars_block(font, block_buffer.get(), block_size); break;
						case 5: read_kerning_block(font, block_buffer.get(), block_size); break;
						default:
							ALERT("unexpected block_type:{} found in {}", block_type, font.get_address());
						}
					}
				}
			}
		}
	}

	void bm_font_reader::read_info_block(font& font, const char* block_buffer, unsigned int block_size) {
		UNUSED_PARAMETER(block_size);

		using WORD = unsigned short;
		using BYTE = unsigned char;

		#pragma pack(push)
		#pragma pack(1)
		struct info_block_data {
			WORD fontSize;
			BYTE reserved : 4;
			BYTE bold : 1;
			BYTE italic : 1;
			BYTE unicode : 1;
			BYTE smooth : 1;
			BYTE charSet;
			WORD stretchH;
			BYTE aa;
			BYTE paddingUp;
			BYTE paddingRight;
			BYTE paddingDown;
			BYTE paddingLeft;
			BYTE spacingHoriz;
			BYTE spacingVert;
			BYTE outline;         // Added with version 2
			char fontName[1];
		};
		#pragma pack(pop)

		const info_block_data* data = reinterpret_cast<const info_block_data*>(block_buffer);
		font._font_height = data->fontSize;
		if (data->unicode != 1) {
			ALERT("unicode flag not set for '{}'", font.get_address());
		}

	}

	void bm_font_reader::read_common_block(font& font, const char* block_buffer, unsigned int block_size) {
		UNUSED_PARAMETER(block_size);

		using WORD = unsigned short;
		using BYTE = unsigned char;

		#pragma pack(push)
		#pragma pack(1)
		struct common_block_data {
			WORD lineHeight;
			WORD base;
			WORD scaleW;
			WORD scaleH;
			WORD pages;
			BYTE packed : 1;
			BYTE reserved : 7;
			BYTE alphaChnl;
			BYTE redChnl;
			BYTE greenChnl;
			BYTE blueChnl;
		};
		#pragma pack(pop)

		const common_block_data* data = reinterpret_cast<const common_block_data*>(block_buffer);

		font._line_height = data->lineHeight;
		font._line_top_to_base_distance = data->base;
		font._page_size = size(data->scaleW, data->scaleH);
		font._page_texture_ids.reserve(data->pages);

	}

	void bm_font_reader::read_pages_block(font& font, const char* block_buffer, unsigned int block_size) {

		#pragma pack(push)
		#pragma pack(1)
		struct pages_block_data {
			char pageNames[1];
		};
		#pragma pack(pop)

		const pages_block_data* data = reinterpret_cast<const pages_block_data*>(block_buffer);

		unsigned int page_name_pos = 0;
		while (page_name_pos < block_size) {
			const char* page_name = &data->pageNames[page_name_pos];
			page_name_pos += get_string_length(page_name) + 1;
			
			texture_id page_texture_id;
			page_texture_id.set_id(get_file_name_no_path_no_extension(page_name).c_str(), _stream.get_description().c_str());
			font._page_texture_ids.push_back(page_texture_id);
		}
	}

	void bm_font_reader::read_chars_block(font& font, const char* block_buffer, unsigned int block_size) {

		using DWORD = unsigned long;
		using WORD = unsigned short;
		using BYTE = unsigned char;

		#pragma pack(push)
		#pragma pack(1)
		struct chars_block_data {
			struct char_info {
				DWORD id;
				WORD  x;
				WORD  y;
				WORD  width;
				WORD  height;
				short xoffset;
				short yoffset;
				short xadvance;
				BYTE  page;
				BYTE  chnl;
			} chars[1];
		};
		#pragma pack(pop)

		const chars_block_data* data = reinterpret_cast<const chars_block_data*>(block_buffer);

		unsigned int char_index = 0;
		while ((sizeof(chars_block_data::char_info) * char_index) < block_size) {
			const chars_block_data::char_info& char_info = data->chars[char_index];

			font_glyph glyph;
			glyph._top_left = pointf(char_info.x, char_info.y);
			glyph._size = sizef(char_info.width, char_info.height);
			glyph._offset = pointf(char_info.xoffset, char_info.yoffset);
			glyph._x_stride = char_info.xadvance;
			glyph._page = char_info.page;
			glyph._channel = char_info.chnl;
			glyph._uvs = simple_rect_uvs(font._page_size, rect(point(char_info.x, char_info.y), size(char_info.width, char_info.height)));

			IF_VERIFY(char_info.id != -1) {
				wchar_t key = static_cast<wchar_t>(char_info.id);
				IF_VERIFY(font._glyph_map.find(key) == font._glyph_map.end()) {
					IF_VERIFY(glyph._page >= 0 && glyph._page < font._page_texture_ids.size()) {
						font._glyph_map[key] = glyph;
					}
				}
			}

			char_index += 1;
		}
	}

	void bm_font_reader::read_kerning_block(font& font, const char* block_buffer, unsigned int block_size) {

		using DWORD = unsigned long;

		#pragma pack(push)
		#pragma pack(1)
		struct kerning_block_data {
			struct pair_info {
				DWORD first;
				DWORD second;
				short amount;
			} pairs[1];
		};
		#pragma pack(pop)

		const kerning_block_data* data = reinterpret_cast<const kerning_block_data*>(block_buffer);

		unsigned int pair_index = 0;
		while ((sizeof(kerning_block_data::pair_info) * pair_index) < block_size) {
			const kerning_block_data::pair_info& pair_info = data->pairs[pair_index];
			wchar_t first_char = static_cast<wchar_t>(pair_info.first);
			wchar_t second_char = static_cast<wchar_t>(pair_info.second);

			auto glyph_iter = font._glyph_map.find(first_char);
			IF_VERIFY(glyph_iter != font._glyph_map.end()) {
				auto& kerning_offset_map = glyph_iter->second._kerning_offset_map;

				//note: bm_font seems to allow duplicate offsets for the same kerning pair. weird. just
				//ignore the duplicates.
				if(kerning_offset_map.find(second_char) == kerning_offset_map.end()) {
					kerning_offset_map[second_char] = pair_info.amount;
				}
			}
				
			pair_index += 1;
		}
	}

}
