#pragma once

#include <bgfx/bgfx.h>

namespace solar {

	class bgfx_callback : public bgfx::CallbackI {
	public:
		virtual void fatal(bgfx::Fatal::Enum code, const char* str) override;
		virtual void traceVargs(const char* filePath, uint16_t line, const char* format, va_list argList) override;
		virtual uint32_t cacheReadSize(uint64_t id) override;
		virtual bool cacheRead(uint64_t id, void* data, uint32_t size) override;
		virtual void cacheWrite(uint64_t id, const void* data, uint32_t size) override;
		virtual void screenShot(const char* filePath, uint32_t width, uint32_t height, uint32_t pitch, const void* data, uint32_t size, bool yflip) override;
		virtual void captureBegin(uint32_t width, uint32_t height, uint32_t pitch, bgfx::TextureFormat::Enum format, bool yflip) override;
		virtual void captureEnd() override;
		virtual void captureFrame(const void* data, uint32_t size) override;
	};

}