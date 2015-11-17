#pragma once

#include <bgfx/bgfx.h>

namespace solar {

	class bgfx_shader_embedded_code {
	public:
		const uint8_t* _data;
		size_t _data_size;

	public:
		bgfx_shader_embedded_code()
			: _data(nullptr)
			, _data_size(0) {
		}

		bgfx_shader_embedded_code(const uint8_t* data, size_t data_size)
			: _data(data)
			, _data_size(data_size) {
		}
	};

	template<size_t N>
	bgfx_shader_embedded_code make_bgfx_shader_embedded_code(const uint8_t (&a) [N]) {
		return bgfx_shader_embedded_code(a, N);
	}
	
}