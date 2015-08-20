#pragma once

#include <memory>
#include "solar/rendering/meshes/mesh.h"
#include "solar/rendering/meshes/mesh_texture_params.h"
#include "solar/resources/resource_address.h"
#include "d3d9_headers.h"
#include "d3d9_mesh_material.h"

namespace solar {

	class archive_reader;
	class stream;
	class mesh_def;
	class d3d9_mesh_factory;

	class d3d9_mesh : public mesh {
	private:
		friend class d3d9_mesh_renderer;

	private:
		d3d9_mesh_factory& _factory;
		mesh_texture_params _texture_params;
		resource_address _resource_address;
		bool _are_rendering_resources_created;

		ID3DXMesh* _id3dx_mesh;
		std::unique_ptr<D3DXATTRIBUTERANGE[]> _attribute_table;
		DWORD _attribute_table_size;
		IDirect3DVertexBuffer9* _vertex_buffer;
		IDirect3DIndexBuffer9* _index_buffer;

		std::vector<std::unique_ptr<d3d9_mesh_material>> _materials;

	public:
		d3d9_mesh(d3d9_mesh_factory& factory, const mesh_texture_params& texture_params, const resource_address& resource_address);
		virtual ~d3d9_mesh();

		const resource_address& get_resource_address() const;
		const mesh_texture_params& get_texture_params() const;

		virtual void create_rendering_resources() override;
		virtual void release_rendering_resources() override;
		virtual int get_sub_mesh_count() const override;

		void on_device_created(IDirect3DDevice9* device);
		void on_device_released(IDirect3DDevice9* device);
		void on_device_reset(IDirect3DDevice9* device);
		void on_device_lost(IDirect3DDevice9* device);

	private:
		void load_and_create_device_mesh(IDirect3DDevice9* device);
		void release_device_mesh();
		std::unique_ptr<archive_reader> make_reader(stream& stream);

	private:
		static ID3DXMesh* create_id3dx_mesh_from_mesh_def(IDirect3DDevice9* device, const mesh_def& mesh_def);
		static void clean_validate_optimize_id3dx_mesh(ID3DXMesh** mesh);
	};

}