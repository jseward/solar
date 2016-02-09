#pragma once

#include "solar/io/stream.h"
#include "font.h"

namespace solar {

	//http://www.angelcode.com/products/bmfont/

	class bm_font_reader {
	private:
		stream& _stream;

	public:
		bm_font_reader(stream& stream);

		void read_font(font& font);

	private:
		void read_info_block(font& font, const char* block_buffer, unsigned int block_size);
		void read_common_block(font& font, const char* block_buffer, unsigned int block_size);
		void read_pages_block(font& font, const char* block_buffer, unsigned int block_size);
		void read_chars_block(font& font, const char* block_buffer, unsigned int block_size);
		void read_kerning_block(font& font, const char* block_buffer, unsigned int block_size);
	};


}