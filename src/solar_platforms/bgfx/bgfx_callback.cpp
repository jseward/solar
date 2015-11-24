#include "bgfx_callback.h"

#include "solar/utility/alert.h"
#include "solar/utility/assert.h"
#include "solar/utility/trace.h"
#include "solar/utility/unused_parameter.h"

namespace solar {

	void bgfx_callback::fatal(bgfx::Fatal::Enum code, const char* str) {
		const char* code_str = "";
		switch (code) {
			case bgfx::Fatal::DebugCheck: code_str = "DebugCheck"; break;
			case bgfx::Fatal::MinimumRequiredSpecs: code_str = "MinimumRequiredSpecs"; break;
			case bgfx::Fatal::InvalidShader: code_str = "InvalidShader"; break;
			case bgfx::Fatal::UnableToInitialize: code_str = "UnableToInitialize"; break;
			case bgfx::Fatal::UnableToCreateTexture: code_str = "UnableToCreateTexture"; break;
			case bgfx::Fatal::DeviceLost: code_str = "DeviceLost"; break;
			default: ASSERT(false); break;
		}

		ALERT("bgfx::fatal! [{}] : {}", code_str, str);
	}

	void bgfx_callback::traceVargs(const char* filePath, uint16_t line, const char* format, va_list argList) {
		char buffer[1024];
		::vsprintf_s(buffer, format, argList);
		solar::trace(filePath, line, false, buffer);
	}

	uint32_t bgfx_callback::cacheReadSize(uint64_t id) {
		UNUSED_PARAMETER(id);
		ASSERT(false);//?
		return 0;
	}

	bool bgfx_callback::cacheRead(uint64_t id, void* data, uint32_t size) {
		UNUSED_PARAMETER(id);
		UNUSED_PARAMETER(data);
		UNUSED_PARAMETER(size);
		ASSERT(false);//?
		return false;
	}

	void bgfx_callback::cacheWrite(uint64_t id, const void* data, uint32_t size) {
		UNUSED_PARAMETER(id);
		UNUSED_PARAMETER(data);
		UNUSED_PARAMETER(size);
		ASSERT(false);//?
	}

	void bgfx_callback::screenShot(const char* filePath, uint32_t width, uint32_t height, uint32_t pitch, const void* data, uint32_t size, bool yflip) {
		UNUSED_PARAMETER(filePath);
		UNUSED_PARAMETER(width);
		UNUSED_PARAMETER(height);
		UNUSED_PARAMETER(pitch);
		UNUSED_PARAMETER(data);
		UNUSED_PARAMETER(size);
		UNUSED_PARAMETER(yflip);
		ASSERT(false);//?
	}

	void bgfx_callback::captureBegin(uint32_t width, uint32_t height, uint32_t pitch, bgfx::TextureFormat::Enum format, bool yflip) {
		UNUSED_PARAMETER(width);
		UNUSED_PARAMETER(height);
		UNUSED_PARAMETER(pitch);
		UNUSED_PARAMETER(format);
		UNUSED_PARAMETER(yflip);
		ASSERT(false);//?
	}

	void bgfx_callback::captureEnd() {
		ASSERT(false);//?
	}

	void bgfx_callback::captureFrame(const void* data, uint32_t size) {
		UNUSED_PARAMETER(data);
		UNUSED_PARAMETER(size);
		ASSERT(false);//?
	}

}
