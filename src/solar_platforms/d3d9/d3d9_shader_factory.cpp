#include "d3d9_shader_factory.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
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

	const char* d3d9_shader_factory::DEFAULT_STATES_SHADER_CODE =
		"technique render {\r\n"
		"	pass p0 {\r\n"
		"		CullMode = CCW;\r\n"
		"		FillMode = SOLID;\r\n"
		"		ColorVertex = FALSE;\r\n"
		"		NormalizeNormals = FALSE;\r\n"
		"		ColorWriteEnable = RED|GREEN|BLUE|ALPHA;\r\n"
		"		AlphaTestEnable = TRUE;\r\n"
		"		AlphaFunc = GREATEREQUAL;\r\n"
		"		AlphaRef = 0x08;\r\n"
		"		AlphaBlendEnable = TRUE;\r\n"
		"		SrcBlend = SRCALPHA;\r\n"
		"		DestBlend = INVSRCALPHA;\r\n"
		"		ColorOp[0] = MODULATE;\r\n"
		"		AlphaOp[0] = MODULATE;\r\n"
		"		ColorArg1[0] = TEXTURE;\r\n"
		"		ColorArg2[0] = DIFFUSE;\r\n"
		"		AlphaArg1[0] = TEXTURE;\r\n"
		"		AlphaArg2[0] = DIFFUSE;\r\n"
		"		TexCoordIndex[0] = 0;\r\n"
		"		TextureTransformFlags[0] = DISABLE;\r\n"
		"		ColorOp[1] = DISABLE;\r\n"
		"		AlphaOp[1] = DISABLE;\r\n"
		"		MinFilter[0] = LINEAR;\r\n"
		"		MagFilter[0] = LINEAR;\r\n"
		"		MipFilter[0] = LINEAR;\r\n"
		"		Lighting = TRUE;\r\n"
		"		SpecularEnable = TRUE;\r\n"
		"		ColorVertex = FALSE;\r\n"
		"		AmbientMaterialSource = MATERIAL;\r\n"
		"		DiffuseMaterialSource = MATERIAL;\r\n"
		"		EmissiveMaterialSource = MATERIAL;\r\n"
		"		SpecularMaterialSource = MATERIAL;\r\n"
		"		MaterialAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };\r\n"
		"		MaterialDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };\r\n"
		"		MaterialSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };\r\n"
		"		MaterialEmissive = { 1.0f, 1.0f, 1.0f, 1.0f };\r\n"
		"		MaterialPower = 1.0f;\r\n"
		"		LightEnable[0] = FALSE;\r\n"
		"		LightEnable[1] = FALSE;\r\n"
		"		Ambient	= {1.0f, 1.0f, 1.0f, 1.0f};\r\n"
		"		ZEnable = TRUE;\r\n"
		"		ZWriteEnable = TRUE;\r\n"
		"		Lighting = FALSE;\r\n"
		"		VertexShader = NULL;\r\n"
		"		PixelShader = NULL;\r\n"
		"		SRGBWriteEnable = FALSE;\r\n"
		"		StencilEnable = FALSE;\r\n"
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
		ASSERT(_ID3DXEffectPool == nullptr);
	}

	void d3d9_shader_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
		_context.add_device_event_handler(this);
		ASSERT(_ID3DXEffectPool == nullptr);
		D3D9_VERIFY(::D3DXCreateEffectPool(&_ID3DXEffectPool));
		create_default_states_shader();
	}

	void d3d9_shader_factory::teardown() {
		release_default_states_shader();
		remove_all_shaders();
		d3d9_release_com_object(_ID3DXEffectPool);
		_context.remove_device_event_handler(this);
		_is_setup = false;
	}

	void d3d9_shader_factory::create_default_states_shader() {
		ASSERT(_default_states_shader == nullptr);
		_default_states_shader = std::make_unique<d3d9_shader>(*this, DEFAULT_STATES_SHADER_CODE);
		_default_states_shader->on_device_created(_context.get_device());
		_default_states_shader->on_device_reset(_context.get_device());
	}

	void d3d9_shader_factory::release_default_states_shader() {
		_default_states_shader->on_device_lost(_context.get_device());
		_default_states_shader->on_device_released(_context.get_device());
		_default_states_shader.release();
	}

	void d3d9_shader_factory::set_render_states_to_defaults() {
		_default_states_shader->start_with_flags("render", D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESHADERSTATE | D3DXFX_DONOTSAVESAMPLERSTATE);
		_default_states_shader->stop();
	}

	void d3d9_shader_factory::remove_all_shaders() {
		for (auto& iter : _shaders) {
			iter.second->on_device_lost(_context.get_device());
			iter.second->on_device_released(_context.get_device());
		}
		_shaders.clear();
		_caching_context.increment();
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
		}
		else {
			new_shader = new d3d9_shader(*this, FALLBACK_SHADER_CODE);
		}
		_shaders[id] = std::unique_ptr<d3d9_shader>(new_shader);

		new_shader->on_device_created(_context.get_device());
		new_shader->on_device_reset(_context.get_device());

		return new_shader;
	}

	const resource_factory_caching_context& d3d9_shader_factory::get_caching_context() const {
		return _caching_context;
	}

	void d3d9_shader_factory::on_device_created(IDirect3DDevice9* device) {
		if (_default_states_shader != nullptr) {
			_default_states_shader->on_device_created(device);
		}

		for (auto& iter : _shaders) {
			iter.second->on_device_created(device);
		}
	}

	void d3d9_shader_factory::on_device_released(IDirect3DDevice9* device) {
		if (_default_states_shader != nullptr) {
			_default_states_shader->on_device_released(device);
		}

		for (auto& iter : _shaders) {
			iter.second->on_device_released(device);
		}
	}

	void d3d9_shader_factory::on_device_reset(IDirect3DDevice9* device) {
		if (_default_states_shader != nullptr) {
			_default_states_shader->on_device_reset(device);
		}

		for (auto& iter : _shaders) {
			iter.second->on_device_reset(device);
		}
	}

	void d3d9_shader_factory::on_device_lost(IDirect3DDevice9* device) {
		if (_default_states_shader != nullptr) {
			_default_states_shader->on_device_lost(device);
		}

		for (auto& iter : _shaders) {
			iter.second->on_device_lost(device);
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

}