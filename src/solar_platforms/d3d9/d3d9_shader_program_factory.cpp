#include "d3d9_shader_program_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
#include "solar/containers/container_helpers.h"
#include "solar/strings/string_helpers.h"
#include "d3d9_verify.h"
#include "d3d9_release_com_object.h"
#include "d3d9_string_convert.h"
#include "d3d9_context.h"

namespace solar {

	const char* d3d9_shader_program_factory::FALLBACK_SHADER_CODE =
		"technique render {\r\n"
		"	pass p0 {\r\n"
		"		VertexShader = NULL;\r\n"
		"		PixelShader = NULL;\r\n"
		"	}\r\n"
		"}\r\n"
		"";

	d3d9_shader_program_factory::d3d9_shader_program_factory(d3d9_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false)
		, _is_debug_shaders_enabled(false)
		, _ID3DXEffectPool(nullptr) {

		_resource_mapped_memory = std::make_unique<resource_mapped_memory>();
		_resource_mapped_memory->set_fixed_buffer_size_in_mb(1.f);
	}

	d3d9_shader_program_factory::~d3d9_shader_program_factory() {
		ASSERT(!_is_setup);
		ASSERT(_shader_programs.empty());
		ASSERT(_embedded_code_shader_programs.empty());
		ASSERT(_ID3DXEffectPool == nullptr);
	}

	void d3d9_shader_program_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
		ASSERT(_ID3DXEffectPool == nullptr);
		D3D9_VERIFY(::D3DXCreateEffectPool(&_ID3DXEffectPool));
	}

	void d3d9_shader_program_factory::teardown() {
		remove_all_shader_programs();
		d3d9_release_com_object(_ID3DXEffectPool);
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	d3d9_shader_program* d3d9_shader_program_factory::create_embedded_code_shader_program(const char* embedded_code) {
		auto new_program = new d3d9_shader_program(*this, embedded_code);
		new_program->on_device_created(_context.get_device());
		new_program->on_device_reset(_context.get_device());
		_embedded_code_shader_programs.push_back(new_program);
		return new_program;
	}

	void d3d9_shader_program_factory::release_embedded_code_shader_program(d3d9_shader_program* program) {
		program->on_device_lost(_context.get_device());
		program->on_device_released(_context.get_device());
		find_and_erase(_embedded_code_shader_programs, program);
		delete program;
	}

	void d3d9_shader_program_factory::remove_all_shader_programs() {
		_resource_system.end_watching_resources(this);
		for (auto& iter : _shader_programs) {
			iter.second->on_device_lost(_context.get_device());
			iter.second->on_device_released(_context.get_device());
		}
		_shader_programs.clear();
		_shader_program_caching_context.increment();
	}

	shader_program* d3d9_shader_program_factory::get_shader_program(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _shader_programs.find(id);
		if (iter != _shader_programs.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address_to_file("shader", "shaders", ".fxo", id.c_str(), id_source_description.c_str());
		d3d9_shader_program* new_program = nullptr;
		if (!address.empty()) {
			new_program = new d3d9_shader_program(*this, address);
			_resource_system.begin_watching_resource(this, address, new_program);
		}
		else {
			new_program = new d3d9_shader_program(*this, FALLBACK_SHADER_CODE);
		}
		_shader_programs[id] = std::unique_ptr<d3d9_shader_program>(new_program);

		new_program->on_device_created(_context.get_device());
		new_program->on_device_reset(_context.get_device());

		return new_program;
	}

	const resource_factory_caching_context& d3d9_shader_program_factory::get_shader_program_caching_context() const {
		return _shader_program_caching_context;
	}

	void d3d9_shader_program_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _shader_programs) {
			iter.second->on_device_created(device);
		}

		for (auto& program : _embedded_code_shader_programs) {
			program->on_device_created(device);
		}
	}

	void d3d9_shader_program_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _shader_programs) {
			iter.second->on_device_released(device);
		}

		for (auto& program : _embedded_code_shader_programs) {
			program->on_device_released(device);
		}
	}

	void d3d9_shader_program_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _shader_programs) {
			iter.second->on_device_reset(device);
		}

		for (auto& program : _embedded_code_shader_programs) {
			program->on_device_reset(device);
		}
	}

	void d3d9_shader_program_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _shader_programs) {
			iter.second->on_device_lost(device);
		}

		for (auto& program : _embedded_code_shader_programs) {
			program->on_device_lost(device);
		}
	}

	ID3DXEffectPool* d3d9_shader_program_factory::get_ID3DXEffectPool() {
		return _ID3DXEffectPool;
	}

	DWORD d3d9_shader_program_factory::get_d3dxcreateeffect_flags() const {
		DWORD flags = D3DXSHADER_PARTIALPRECISION;
		if (_is_debug_shaders_enabled) {
			flags |= D3DXSHADER_DEBUG;
			flags |= D3DXSHADER_SKIPOPTIMIZATION;

			//These flags cause the shader to not work properly, but don't raise any error messages. Not sure why.
			//flags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
			//flags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
		}
		else {
			flags |= D3DXSHADER_OPTIMIZATION_LEVEL3;
		}
		return flags;
	}

	resource_system& d3d9_shader_program_factory::get_resource_system() {
		return _resource_system;
	}

	resource_mapped_memory& d3d9_shader_program_factory::get_resource_mapped_memory() {
		return *_resource_mapped_memory.get();
	}

	void d3d9_shader_program_factory::on_file_changed(const std::string& path, void* data) {
		auto program = reinterpret_cast<d3d9_shader_program*>(data);
		ASSERT(program->get_resource_address().get_path() == path);
		program->on_device_lost(_context.get_device());
		program->on_device_released(_context.get_device());
		program->on_device_created(_context.get_device());
		program->on_device_reset(_context.get_device());
	}

}
