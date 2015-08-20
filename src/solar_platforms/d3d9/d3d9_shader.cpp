#include "d3d9_shader.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"
#include "d3d9_verify.h"
#include "d3d9_texture.h"
#include "d3d9_shader_factory.h"
#include "d3d9_release_com_object.h"
#include "d3d9_string_convert.h"

namespace solar {

	d3d9_shader::d3d9_shader(d3d9_shader_factory& factory, const resource_address& resource_address)
		: _factory(factory)
		, _resource_address(resource_address)
		, _embeded_code(nullptr) 
		, _ID3DXEffect(nullptr)
		, _is_started(false)
		, _is_within_pass(false)
		, _has_changes_to_commit(false) {

		ASSERT(!resource_address.empty());
	}

	d3d9_shader::d3d9_shader(d3d9_shader_factory& factory, const char* embeded_code)
		: _factory(factory) 
		, _resource_address()
		, _embeded_code(embeded_code) 
		, _ID3DXEffect(nullptr)
		, _is_started(false)
		, _is_within_pass(false)
		, _has_changes_to_commit(false) {
	}

	d3d9_shader::~d3d9_shader() {
		ASSERT(_ID3DXEffect == nullptr);
		ASSERT(!_is_started);
		ASSERT(!_is_within_pass);
		ASSERT(!_has_changes_to_commit);
	}

	const resource_address& d3d9_shader::get_resource_address() const {
		return _resource_address;
	}

	const char* d3d9_shader::get_embeded_code() const {
		return _embeded_code;
	}
	
	void d3d9_shader::start_with_flags(const char* technique, DWORD flags) {
		IF_VERIFY(_ID3DXEffect != nullptr) { //D3DXCreateEffect can fail
			ASSERT(!_is_started);
			_is_started = true;

			auto iter = _technique_handles.find(technique);
			if (iter == _technique_handles.end()) {
				ALERT("d3d9_shader technique not found.\n\nshader: {}\ntechnique: {}", _resource_address.to_string(), technique);
			}
			else {
				D3D9_VERIFY(_ID3DXEffect->SetTechnique(iter->second));
			}

			UINT pass_count = 0;
			D3D9_VERIFY(_ID3DXEffect->Begin(&pass_count, flags));

			//When shared params were removed the new method to replace them is to set params on all shaders even if they are not needed. Because of this
			//some shaders may not know they should have committed param changes, so do it for them gracefully.
			commit_param_changes();

			ASSERT(pass_count == 1); //pass management handled internal, no shaders can be authored with multiple passes, too much complexity.
			begin_pass(0);
		}
	}

	void d3d9_shader::start(const char* technique) {
		start_with_flags(technique, 0);
	}

	void d3d9_shader::stop() {
		IF_VERIFY(_ID3DXEffect != nullptr) { //D3DXCreateEffect can fail
			ASSERT(_is_started);
			_is_started = false;

			end_pass();

			D3D9_VERIFY(_ID3DXEffect->End());
		}
	}

	void d3d9_shader::begin_pass(unsigned int pass) {
		ASSERT(!_is_within_pass);
		_is_within_pass = true;
		D3D9_VERIFY(_ID3DXEffect->BeginPass(pass));
	}

	void d3d9_shader::end_pass() {
		ASSERT(_is_within_pass);
		_is_within_pass = false;
		D3D9_VERIFY(_ID3DXEffect->EndPass());
	}

	void d3d9_shader::commit_param_changes() {
		if (_has_changes_to_commit) {
			D3D9_VERIFY(_ID3DXEffect->CommitChanges());
			_has_changes_to_commit = false;
		}
	}

	void d3d9_shader::forget_param_changes() {
		_has_changes_to_commit = false;
	}

	bool d3d9_shader::set_bool(const char* name, bool value) {
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetBool(handle, value));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	bool d3d9_shader::set_float(const char* name, float value) {
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetFloat(handle, value));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	bool d3d9_shader::set_float_array(const char* name, const float* values, unsigned int count) {
		UNUSED_PARAMETER(count);
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetFloatArray(handle, values, 2));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	bool d3d9_shader::set_mat44(const char* name, const mat44& value) {
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetMatrix(handle, reinterpret_cast<const D3DXMATRIX*>(&value)));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	bool d3d9_shader::set_texture(const char* name, texture& texture) {
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetTexture(handle, static_cast<d3d9_texture&>(texture).request_IDirect3DTexture9()));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	bool d3d9_shader::set_platform_texture(const char* name, void* texture) {
		D3DXHANDLE handle = get_param_to_change(name);
		if (handle != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->SetTexture(handle, static_cast<IDirect3DBaseTexture9*>(texture)));
			_has_changes_to_commit = true;
			return true;
		}
		return false;
	}

	void d3d9_shader::map_technique_handles() {
		ASSERT(_technique_handles.empty());
		for (UINT i = 0; i < _desc.Techniques; ++i) {
			D3DXHANDLE handle = _ID3DXEffect->GetTechnique(i);
			IF_VERIFY(handle != nullptr) {
				D3DXTECHNIQUE_DESC technique_desc;
				D3D9_VERIFY(_ID3DXEffect->GetTechniqueDesc(handle, &technique_desc));
				_technique_handles[technique_desc.Name] = handle;
			}
		}
	}

	void d3d9_shader::map_param_handles() {
		ASSERT(_param_handles.empty());
		for (UINT i = 0; i < _desc.Parameters; ++i) {
			D3DXHANDLE handle = _ID3DXEffect->GetParameter(NULL, i);
			IF_VERIFY(handle != nullptr) {
				D3DXPARAMETER_DESC parameter_desc;
				D3D9_VERIFY(_ID3DXEffect->GetParameterDesc(handle, &parameter_desc));
				_param_handles[parameter_desc.Name] = handle;
			}
		}
	}

	D3DXHANDLE d3d9_shader::get_param_to_change(const char* name) {
		auto iter = _param_handles.find(name);
		if (iter != _param_handles.end()) {
			return iter->second;
		}
		return nullptr;
	}

	void d3d9_shader::on_device_created(IDirect3DDevice9* device) {
		ASSERT(_ID3DXEffect == nullptr);

		if (_embeded_code != nullptr) {
			UINT size = (UINT)strlen(_embeded_code) + 1;
			D3D9_VERIFY(::D3DXCreateEffect(
				device,
				_embeded_code,
				size,
				NULL,
				NULL,
				_factory.get_d3dxcreateeffect_flags() | D3DXFX_NOT_CLONEABLE,
				_factory.get_ID3DXEffectPool(),
				&_ID3DXEffect,
				NULL));
		}
		else {
			auto& resource_mapped_memory = _factory.get_resource_mapped_memory();
			resource_mapped_memory.lock();
			if (_factory.get_resource_system().read_to_mapped_memory(resource_mapped_memory, _resource_address)) {
				ID3DXBuffer* error_buffer = nullptr;
				HRESULT hr = ::D3DXCreateEffect(
					device,
					resource_mapped_memory.get_buffer(),
					resource_mapped_memory.get_buffer_size(),
					NULL,
					NULL,
					_factory.get_d3dxcreateeffect_flags() | D3DXFX_NOT_CLONEABLE,
					_factory.get_ID3DXEffectPool(),
					&_ID3DXEffect,
					&error_buffer);
				if (FAILED(hr)) {
					ALERT("Failed to create D3DXEffect\n\naddress: {}\nerror: {}", _resource_address.to_string(), ID3DXBuffer_to_string(error_buffer));
				}
				d3d9_release_com_object(error_buffer);
			}
			resource_mapped_memory.unlock();
		}

		if (_ID3DXEffect != nullptr) {
			D3D9_VERIFY(_ID3DXEffect->GetDesc(&_desc));
		}

		map_technique_handles();
		map_param_handles();
	}

	void d3d9_shader::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		_technique_handles.clear();
		_param_handles.clear();
		d3d9_release_com_object(_ID3DXEffect);
	}

	void d3d9_shader::on_device_reset(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		if (_ID3DXEffect != nullptr) {
			_ID3DXEffect->OnResetDevice();
		}
	}

	void d3d9_shader::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		if (_ID3DXEffect != nullptr) {
			_ID3DXEffect->OnLostDevice();
		}
	}

}
