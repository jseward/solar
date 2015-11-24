#include "bgfx_shader_program.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_build.h"
#include "solar/resources/resource_system.h"
#include "bgfx_context.h"
#include "bgfx_shader_program_factory.h"

namespace solar {

	bgfx_shader_program::bgfx_shader_program(bgfx_shader_program_factory& factory, const resource_address& resource_address)
		: _factory(factory)
		, _resource_address(resource_address) 

		, _program_handle(BGFX_INVALID_HANDLE) {
	}

	bgfx_shader_program::bgfx_shader_program(bgfx_shader_program_factory& factory, bgfx_shader_embedded_code vs_embedded_code, bgfx_shader_embedded_code fs_embedded_code)
		: _factory(factory)
		, _vs_embedded_code(vs_embedded_code)
		, _fs_embedded_code(fs_embedded_code)

		, _program_handle(BGFX_INVALID_HANDLE) {
	}

	bgfx_shader_program::~bgfx_shader_program() {
		destroy_program_handle();
	}

	void bgfx_shader_program::destroy_program_handle() {
		if (isValid(_program_handle)) {
			bgfx::destroyProgram(_program_handle);
			_program_handle = BGFX_INVALID_HANDLE;
		}
	}

	const resource_address& bgfx_shader_program::get_resource_address() const {
		return _resource_address;
	}

	void bgfx_shader_program::load() {
		destroy_program_handle();

		IF_VERIFY(!_resource_address.empty()) {
			_factory.get_resource_system().read_object_as_json(_program_def, _resource_address);

			auto vsh = load_shader(_factory.get_resource_system(), _resource_address, bgfx_shader_type::VERTEX, _program_def._vertex_shader_name);
			auto fsh = load_shader(_factory.get_resource_system(), _resource_address, bgfx_shader_type::FRAGMENT, _program_def._fragment_shader_name);

			_program_handle = bgfx::createProgram(vsh, fsh, true);
		}
	}

	void bgfx_shader_program::create_embedded() {
		IF_VERIFY(_vs_embedded_code._data != nullptr && _fs_embedded_code._data != nullptr) {
			auto vsh = bgfx::createShader(bgfx::makeRef(_vs_embedded_code._data, _vs_embedded_code._data_size));
			auto fsh = bgfx::createShader(bgfx::makeRef(_fs_embedded_code._data, _fs_embedded_code._data_size));

			ASSERT(!isValid(_program_handle))
			_program_handle = bgfx::createProgram(vsh, fsh, true);
		}
	}

	bgfx::ShaderHandle bgfx_shader_program::load_shader(resource_system& resource_system, const resource_address& def_resource_address, bgfx_shader_type shader_type, const std::string& name) {
		auto address = resource_system.resolve_address("shader", "shaders", get_shader_extension(shader_type).c_str(), name.c_str(), def_resource_address.to_string().c_str());
		if (!address.empty()) {
			auto stream = resource_system.open_stream_to_read(address);
			if (stream != nullptr) {
				auto size = stream->get_size();
				
				const bgfx::Memory* memory = bgfx::alloc(size + 1);
				stream->read_bytes(reinterpret_cast<char*>(memory->data), memory->size);
				memory->data[memory->size - 1] = '\0';

				resource_system.close_stream(stream);

				return bgfx::createShader(memory);
			}
		}

		return BGFX_INVALID_HANDLE;
	}

	std::string bgfx_shader_program::get_shader_extension(bgfx_shader_type shader_type) {
		
		const char* shader_type_string = "";
		switch (shader_type) {
			case bgfx_shader_type::VERTEX: shader_type_string = "vs"; break;
			case bgfx_shader_type::FRAGMENT: shader_type_string = "fs"; break;
			default: ASSERT(false); break;
		}

		const char* render_type_string = "";
		switch (bgfx::getRendererType()) {
			case bgfx::RendererType::Direct3D9: render_type_string = "d3d9"; break;
			case bgfx::RendererType::Direct3D11: render_type_string = "d3d11"; break;
			case bgfx::RendererType::Direct3D12: render_type_string = "d3d12"; break;
			case bgfx::RendererType::OpenGL: render_type_string = "glsl"; break;
			case bgfx::RendererType::OpenGLES: render_type_string = "glsles"; break;;
			case bgfx::RendererType::Metal: render_type_string = "metal"; break;
			default: ASSERT(false); break;
		}
		
		return build_string(".{}_{}", shader_type_string, render_type_string);
	}

	void bgfx_shader_program::start() {
		//null-op of bgfx
	}

	void bgfx_shader_program::stop() {
		//null-op of bgfx
	}

	void bgfx_shader_program::commit_param_changes() {
		//todo
	}

	void bgfx_shader_program::forget_param_changes() {
		//todo
	}

	bool bgfx_shader_program::set_bool(const char* name, bool value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_float(const char* name, float value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_float_array(const char* name, const float* values, unsigned int count) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(values);
		UNUSED_PARAMETER(count);
		return false;
	}

	bool bgfx_shader_program::set_mat44(const char* name, const mat44& value) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(value);
		return false;
	}

	bool bgfx_shader_program::set_texture(const char* name, texture& texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

	bool bgfx_shader_program::set_platform_texture(const char* name, void* texture) {
		//todo
		UNUSED_PARAMETER(name);
		UNUSED_PARAMETER(texture);
		return false;
	}

	bgfx::ProgramHandle bgfx_shader_program::get_program_handle() const {
		return _program_handle;
	}

}
