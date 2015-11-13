#include "d3d9_shader_factory.h"

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

	const char* d3d9_shader_factory::FALLBACK_SHADER_CODE =
		"technique render {\r\n"
		"	pass p0 {\r\n"
		"		VertexShader = NULL;\r\n"
		"		PixelShader = NULL;\r\n"
		"	}\r\n"
		"}\r\n"
		"";

	d3d9_shader_factory::d3d9_shader_factory(d3d9_context& context, resource_system& resource_system)
		: _context(context)
		, _resource_system(resource_system)
		, _is_setup(false)
		, _is_debug_shaders_enabled(false)
		, _ID3DXEffectPool(nullptr) {

		_resource_mapped_memory = std::make_unique<resource_mapped_memory>();
		_resource_mapped_memory->set_fixed_buffer_size_in_mb(1.f);
	}

	d3d9_shader_factory::~d3d9_shader_factory() {
		ASSERT(!_is_setup);
		ASSERT(_shaders.empty());
		ASSERT(_embeded_code_shaders.empty());
		ASSERT(_ID3DXEffectPool == nullptr);
	}

	void d3d9_shader_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
		ASSERT(_ID3DXEffectPool == nullptr);
		D3D9_VERIFY(::D3DXCreateEffectPool(&_ID3DXEffectPool));
	}

	void d3d9_shader_factory::teardown() {
		remove_all_shaders();
		d3d9_release_com_object(_ID3DXEffectPool);
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	d3d9_shader* d3d9_shader_factory::create_embeded_code_shader(const char* embeded_code) {
		auto new_shader = new d3d9_shader(*this, embeded_code);
		new_shader->on_device_created(_context.get_device());
		new_shader->on_device_reset(_context.get_device());
		_embeded_code_shaders.push_back(new_shader);
		return new_shader;
	}

	void d3d9_shader_factory::release_embeded_code_shader(d3d9_shader* shader) {
		shader->on_device_lost(_context.get_device());
		shader->on_device_released(_context.get_device());
		find_and_erase(_embeded_code_shaders, shader);
		delete shader;
	}

	void d3d9_shader_factory::remove_all_shaders() {
		_resource_system.end_watching_resources(this);
		for (auto& iter : _shaders) {
			iter.second->on_device_lost(_context.get_device());
			iter.second->on_device_released(_context.get_device());
		}
		_shaders.clear();
		_shader_caching_context.increment();
	}

	shader* d3d9_shader_factory::get_shader(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _shaders.find(id);
		if (iter != _shaders.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address("shader", "shaders", ".fxo", id.c_str(), id_source_description.c_str());
		d3d9_shader* new_shader = nullptr;
		if (!address.empty()) {
			new_shader = new d3d9_shader(*this, address);
			_resource_system.begin_watching_resource(this, address, new_shader);
		}
		else {
			new_shader = new d3d9_shader(*this, FALLBACK_SHADER_CODE);
		}
		_shaders[id] = std::unique_ptr<d3d9_shader>(new_shader);

		new_shader->on_device_created(_context.get_device());
		new_shader->on_device_reset(_context.get_device());

		return new_shader;
	}

	const resource_factory_caching_context& d3d9_shader_factory::get_shader_caching_context() const {
		return _shader_caching_context;
	}

	void d3d9_shader_factory::on_device_created(IDirect3DDevice9* device) {
		for (auto& iter : _shaders) {
			iter.second->on_device_created(device);
		}

		for (auto& shader : _embeded_code_shaders) {
			shader->on_device_created(device);
		}
	}

	void d3d9_shader_factory::on_device_released(IDirect3DDevice9* device) {
		for (auto& iter : _shaders) {
			iter.second->on_device_released(device);
		}

		for (auto& shader : _embeded_code_shaders) {
			shader->on_device_released(device);
		}
	}

	void d3d9_shader_factory::on_device_reset(IDirect3DDevice9* device) {
		for (auto& iter : _shaders) {
			iter.second->on_device_reset(device);
		}

		for (auto& shader : _embeded_code_shaders) {
			shader->on_device_reset(device);
		}
	}

	void d3d9_shader_factory::on_device_lost(IDirect3DDevice9* device) {
		for (auto& iter : _shaders) {
			iter.second->on_device_lost(device);
		}

		for (auto& shader : _embeded_code_shaders) {
			shader->on_device_lost(device);
		}
	}

	ID3DXEffectPool* d3d9_shader_factory::get_ID3DXEffectPool() {
		return _ID3DXEffectPool;
	}

	DWORD d3d9_shader_factory::get_d3dxcreateeffect_flags() const {
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

	resource_system& d3d9_shader_factory::get_resource_system() {
		return _resource_system;
	}

	resource_mapped_memory& d3d9_shader_factory::get_resource_mapped_memory() {
		return *_resource_mapped_memory.get();
	}

	void d3d9_shader_factory::on_file_changed(const std::string& path, void* data) {
		d3d9_shader* shader = reinterpret_cast<d3d9_shader*>(data);
		ASSERT(shader->get_resource_address().get_file_path() == path);
		shader->on_device_lost(_context.get_device());
		shader->on_device_released(_context.get_device());
		shader->on_device_created(_context.get_device());
		shader->on_device_reset(_context.get_device());
	}

}