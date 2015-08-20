#pragma once

#include "solar/utility/assert.h"
#include "d3d9_verify.h"
#include "d3d9_release_com_object.h"
#include "d3d9_dynamic_index_buffer.h"

namespace solar {

	template<typename Vertex> 
	class d3d9_dynamic_vertex_buffer {
	private:
		IDirect3DVertexBuffer9* _idirect3dvertexbuffer9;
		int _max_vertices;
		int _vertices_begin;
		int _vertices_used;
		bool _should_discard_on_next_lock;
		bool _is_locked;

	public:
		d3d9_dynamic_vertex_buffer();
		~d3d9_dynamic_vertex_buffer();

		void set_max_vertices(int max_vertices);

		Vertex* lock(int vertices_required);
		void unlock();

		void render_indexed_tris(IDirect3DDevice9* idirect3ddevice9, d3d9_dynamic_index_buffer& index_buffer);

		void move_past_contents();
		void discard_contents();

		int get_vertices_available() const;
		int get_vertices_used() const;

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);

	private:
		bool prepare_render(IDirect3DDevice9* idirect3ddevice9);
	};

	template<typename Vertex>
	d3d9_dynamic_vertex_buffer<Vertex>::d3d9_dynamic_vertex_buffer() 
		: _idirect3dvertexbuffer9(nullptr)
		, _max_vertices(0)
		, _vertices_begin(0)
		, _vertices_used(0) 
		, _should_discard_on_next_lock(false)
		, _is_locked(false) {
	}

	template<typename Vertex>
	d3d9_dynamic_vertex_buffer<Vertex>::~d3d9_dynamic_vertex_buffer() {
		ASSERT(_idirect3dvertexbuffer9 == nullptr);
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::set_max_vertices(int max_vertices) {
		ASSERT(_idirect3dvertexbuffer9 == nullptr);
		ASSERT(max_vertices > 0);
		_max_vertices = max_vertices;
	}

	template<typename Vertex>
	Vertex* d3d9_dynamic_vertex_buffer<Vertex>::lock(int vertices_required) {
		ASSERT(vertices_required <= get_vertices_available());
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

		Vertex* vertices = nullptr;
		D3D9_VERIFY(_idirect3dvertexbuffer9->Lock(
			_vertices_begin * sizeof(Vertex),
			vertices_required * sizeof(Vertex),
			reinterpret_cast<void**>(&vertices),
			flags));
		vertices += _vertices_used;
		_vertices_used += vertices_required;

		return vertices;
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::unlock() {
		ASSERT(_is_locked);
		_is_locked = false;
		D3D9_VERIFY(_idirect3dvertexbuffer9->Unlock());
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::render_indexed_tris(IDirect3DDevice9* device, d3d9_dynamic_index_buffer& index_buffer) {
		if (prepare_render(device)) {
			D3D9_VERIFY(device->SetIndices(index_buffer.get_idirect3dindexbuffer9()));
			D3D9_VERIFY(device->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST, 
				_vertices_begin, 
				0U, 
				_vertices_used, 
				index_buffer.get_indices_begin(), 
				(index_buffer.get_indices_used() / 3)));
		}
	}

	template<typename Vertex>
	bool d3d9_dynamic_vertex_buffer<Vertex>::prepare_render(IDirect3DDevice9* device) {
		if (_vertices_used > 0) {
			D3D9_VERIFY(device->SetStreamSource(0U, _idirect3dvertexbuffer9, 0U, sizeof(Vertex)));
			D3D9_VERIFY(device->SetFVF(Vertex::FVF));
			return true;
		}
		return false;
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::move_past_contents() {
		_vertices_begin += _vertices_used;
		_vertices_used = 0;
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::discard_contents() {
		_should_discard_on_next_lock = true;
		_vertices_begin = 0;
		_vertices_used = 0;
	}

	template<typename Vertex>
	int d3d9_dynamic_vertex_buffer<Vertex>::get_vertices_available() const {
		return _max_vertices - _vertices_used - _vertices_begin;
	}

	template<typename Vertex>
	int d3d9_dynamic_vertex_buffer<Vertex>::get_vertices_used() const {
		return _vertices_used;
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::on_device_reset(IDirect3DDevice9* device) {
		const UINT vertex_format_size = D3DXGetFVFVertexSize(Vertex::FVF);
		const UINT vertex_struct_size = sizeof(Vertex);
		ASSERT(vertex_format_size == vertex_struct_size);

		ASSERT(_max_vertices > 0);
		ASSERT(_idirect3dvertexbuffer9 == NULL);
		D3D9_VERIFY(device->CreateVertexBuffer(
			_max_vertices * sizeof(Vertex),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			Vertex::FVF,
			D3DPOOL_DEFAULT,
			&_idirect3dvertexbuffer9,
			nullptr));
	}

	template<typename Vertex>
	void d3d9_dynamic_vertex_buffer<Vertex>::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		d3d9_release_com_object(_idirect3dvertexbuffer9);
	}

}