#pragma once

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "d3d9_verify.h"
#include "d3d9_release_com_object.h"

namespace solar {

	class d3d9_dynamic_index_buffer {
	private:
		IDirect3DIndexBuffer9* _idirect3dindexbuffer9;
		int _max_indices;
		int _indices_begin;
		int _indices_used;
		bool _should_discard_on_next_lock;
		bool _is_locked;

	public:
		d3d9_dynamic_index_buffer();
		~d3d9_dynamic_index_buffer();

		void set_max_indices(int max_indices);

		WORD* lock(int indices_required);
		void unlock();

		void move_past_contents();
		void discard_contents();

		IDirect3DIndexBuffer9* get_idirect3dindexbuffer9();
		int get_indices_available() const;
		int get_indices_begin() const;
		int get_indices_used() const;

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);
	};

	inline d3d9_dynamic_index_buffer::d3d9_dynamic_index_buffer() 
		: _idirect3dindexbuffer9(nullptr)
		, _max_indices(0)
		, _indices_begin(0)
		, _indices_used(0)
		, _should_discard_on_next_lock(false)
		, _is_locked(false) {
	}

	inline d3d9_dynamic_index_buffer::~d3d9_dynamic_index_buffer() {
		ASSERT(_idirect3dindexbuffer9 == nullptr);
	}

	inline void d3d9_dynamic_index_buffer::set_max_indices(int max_indices) {
		_max_indices = max_indices;
	}

	inline WORD* d3d9_dynamic_index_buffer::lock(int indices_required) {
		ASSERT(indices_required <= get_indices_available());
		ASSERT(!_is_locked);
		_is_locked = true;

		DWORD flags = 0UL;
		if (_should_discard_on_next_lock) {
			_should_discard_on_next_lock = false;
			flags |= D3DLOCK_DISCARD;
		}
		else {
			flags |= D3DLOCK_NOOVERWRITE;
		}

		//D3DLOCK_NOSYSLOCK should only be needed for debugging on Windows 9x systems.
		//http://msdn.microsoft.com/en-us/library/bb219721.aspx
		//flags |= D3DLOCK_NOSYSLOCK;

		WORD* indices = nullptr;
		D3D9_VERIFY(_idirect3dindexbuffer9->Lock(
			_indices_begin * sizeof(WORD),
			indices_required * sizeof(WORD),
			reinterpret_cast<void**>(&indices), 
			flags));
		indices += _indices_used;
		_indices_used += indices_required;
		
		return indices;
	}

	inline void d3d9_dynamic_index_buffer::unlock() {
		ASSERT(_is_locked);
		_is_locked = false;
		D3D9_VERIFY(_idirect3dindexbuffer9->Unlock());
	}

	inline void d3d9_dynamic_index_buffer::move_past_contents() {
		_indices_begin += _indices_used;
		_indices_used = 0;
	}

	inline void d3d9_dynamic_index_buffer::discard_contents() {
		_should_discard_on_next_lock = true;
		_indices_begin = 0;
		_indices_used = 0;
	}

	inline IDirect3DIndexBuffer9* d3d9_dynamic_index_buffer::get_idirect3dindexbuffer9() {
		return _idirect3dindexbuffer9;
	}

	inline int d3d9_dynamic_index_buffer::get_indices_available() const {
		return _max_indices - _indices_used - _indices_begin;
	}

	inline int d3d9_dynamic_index_buffer::get_indices_begin() const {
		return _indices_begin;
	}

	inline int d3d9_dynamic_index_buffer::get_indices_used() const {
		return _indices_used;
	}

	inline void d3d9_dynamic_index_buffer::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	inline void d3d9_dynamic_index_buffer::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	inline void d3d9_dynamic_index_buffer::on_device_reset(IDirect3DDevice9* device) {
		ASSERT(_max_indices > 0);
		ASSERT(_idirect3dindexbuffer9 == nullptr);
		D3D9_VERIFY(device->CreateIndexBuffer(
			_max_indices * sizeof(WORD),
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&_idirect3dindexbuffer9,
			nullptr));
	}

	inline void d3d9_dynamic_index_buffer::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		d3d9_release_com_object(_idirect3dindexbuffer9);
	}

}