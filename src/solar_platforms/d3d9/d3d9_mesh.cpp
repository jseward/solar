#include "d3d9_mesh.h"

#include "solar/utility/unused_parameter.h"
#include "solar/utility/assert.h"
#include "solar/resources/resource_system.h"
#include "solar/archiving/binary_archive_reader.h"
#include "solar/archiving/json_archive_reader.h"
#include "solar/rendering/meshes/mesh_def.h"
#include "solar/rendering/meshes/mesh_constants.h"
#include "d3d9_mesh_factory.h"
#include "d3d9_mesh_vertex_declaration.h"
#include "d3d9_mesh_vertex.h"
#include "d3d9_verify.h"
#include "d3d9_context.h"
#include "d3d9_release_com_object.h"

namespace solar {

	d3d9_mesh::d3d9_mesh(d3d9_mesh_factory& factory, const mesh_texture_params& texture_params, const resource_address& resource_address)
		: _factory(factory)
		, _texture_params(texture_params)
		, _resource_address(resource_address)
		, _are_rendering_resources_created(false)
		, _id3dx_mesh(nullptr)
		, _attribute_table(nullptr) 
		, _attribute_table_size(0)
		, _vertex_buffer(nullptr)
		, _index_buffer(nullptr) {
	}

	d3d9_mesh::~d3d9_mesh() {
		ASSERT(_id3dx_mesh == nullptr);
		ASSERT(_attribute_table == nullptr);
		ASSERT(_attribute_table_size == 0);
		ASSERT(_vertex_buffer == nullptr);
		ASSERT(_index_buffer == nullptr);
	}

	const resource_address& d3d9_mesh::get_resource_address() const {
		return _resource_address;
	}

	const mesh_texture_params& d3d9_mesh::get_texture_params() const {
		return _texture_params;
	}

	void d3d9_mesh::create_rendering_resources() {
		if (!_are_rendering_resources_created && !_resource_address.empty()) {
			load_and_create_device_mesh(_factory.get_d3d9_context().get_device());
			_are_rendering_resources_created = true;
		}
	}

	void d3d9_mesh::release_rendering_resources() {
		release_device_mesh();
		_are_rendering_resources_created = false;
	}

	int d3d9_mesh::get_sub_mesh_count() const {
		return _attribute_table_size;
	}

	void d3d9_mesh::on_device_created(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		if (_are_rendering_resources_created) {
			load_and_create_device_mesh(device);
		}
	}

	void d3d9_mesh::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		release_device_mesh();
	}

	void d3d9_mesh::on_device_reset(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_mesh::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_mesh::load_and_create_device_mesh(IDirect3DDevice9* device) {

		auto stream = _factory.get_resource_system().open_stream_to_read(_resource_address);
		if (stream != nullptr) {
			auto reader = make_reader(*stream);
			if (reader != nullptr) {

				//todo - this could be a source of lots of dynamic memory churn as mesh_def is read in, used and discarded. 
				//a potentially better solution is to have mesh_def provide a read_from_archive_to_external function that allows
				//us to specific callbacks and process data as it is read in place. but overly complicated for now.
				mesh_def mesh_def;
				mesh_def.read_from_archive(*reader);

				if (mesh_def._vertices.empty()) {
					//just ignore empty meshes. SinsOfADarkAge had mesh files that have no visible geometry but did have mesh points defined in parent mesh for positioning add-ons.
				}
				else {
					ASSERT(_id3dx_mesh == nullptr);
					_id3dx_mesh = create_id3dx_mesh_from_mesh_def(device, mesh_def);

					if (_id3dx_mesh != nullptr) {
						D3D9_VERIFY(_id3dx_mesh->GetAttributeTable(NULL, &_attribute_table_size));
						ASSERT(_attribute_table_size > 0); // mesh without any attributes?  nothing will render...
						_attribute_table = std::unique_ptr<D3DXATTRIBUTERANGE[]>(new D3DXATTRIBUTERANGE[_attribute_table_size]);
						D3D9_VERIFY(_id3dx_mesh->GetAttributeTable(_attribute_table.get(), &_attribute_table_size));
						D3D9_VERIFY(_id3dx_mesh->GetVertexBuffer(&_vertex_buffer));
						D3D9_VERIFY(_id3dx_mesh->GetIndexBuffer(&_index_buffer));
					}

					ASSERT(_materials.empty());
					_materials.reserve(mesh_def._materials.size());
					for (const auto& material : mesh_def._materials) {
						_materials.push_back(std::make_unique<d3d9_mesh_material>(
							_resource_address.get_file_path(), _texture_params, material));
					}
				}
			}

			_factory.get_resource_system().close_stream(stream);
		}
	}

	void d3d9_mesh::release_device_mesh() {
		_attribute_table.release();
		_attribute_table_size = 0;
		d3d9_release_com_object(_vertex_buffer);
		d3d9_release_com_object(_index_buffer);
		d3d9_release_com_object(_id3dx_mesh);
	}

	std::unique_ptr<archive_reader> d3d9_mesh::make_reader(stream& stream) {
		if (_resource_address.get_file_extension() == BINARY_MESH_FILE_EXTENSION) {
			return std::make_unique<binary_archive_reader>(stream);
		}
		else if (_resource_address.get_file_extension() == JSON_MESH_FILE_EXTENSION) {
			return std::make_unique<json_archive_reader>(stream);
		}
		else {
			ALERT("unexpected mesh file extension : {}", _resource_address.get_file_extension());
		}

		return nullptr;
	}

	ID3DXMesh* d3d9_mesh::create_id3dx_mesh_from_mesh_def(IDirect3DDevice9* device, const mesh_def& mesh_def) {
		ID3DXMesh* id3dx_mesh = nullptr;

		D3D9_VERIFY(::D3DXCreateMesh(
			mesh_def._triangles.size(),
			mesh_def._vertices.size(),
			D3DXMESH_MANAGED,
			d3d9_mesh_vertex_declaration().get_elements_begin(),
			device,
			&id3dx_mesh));

		if (id3dx_mesh != nullptr) {
			d3d9_mesh_vertex* vertices = nullptr;
			D3D9_VERIFY(id3dx_mesh->LockVertexBuffer(0, reinterpret_cast<void**>(&vertices)));
			for (unsigned int i = 0; i < mesh_def._vertices.size(); ++i) {
				vertices[i] = mesh_def._vertices[i];
			}
			D3D9_VERIFY(id3dx_mesh->UnlockVertexBuffer());

			WORD* indices = nullptr;
			D3D9_VERIFY(id3dx_mesh->LockIndexBuffer(0, reinterpret_cast<void**>(&indices)));
			DWORD* attributes = NULL;
			D3D9_VERIFY(id3dx_mesh->LockAttributeBuffer(0, &attributes));
			for (unsigned int i = 0; i < mesh_def._triangles.size(); i++) {
				const auto& triangle = mesh_def._triangles[i];
				indices[(i * 3) + 0] = triangle._vertex_index_0;
				indices[(i * 3) + 1] = triangle._vertex_index_1;
				indices[(i * 3) + 2] = triangle._vertex_index_2;
				attributes[i] = triangle._material_index;
			}
			D3D9_VERIFY(id3dx_mesh->UnlockIndexBuffer());
			D3D9_VERIFY(id3dx_mesh->UnlockAttributeBuffer());

			//this is required or attribute table size will be 0? really should drop dependency on d3dxmesh altogether and
			//just directly create vertex/index buffers.
			clean_validate_optimize_id3dx_mesh(&id3dx_mesh);
		}

		return id3dx_mesh;
	}

	void d3d9_mesh::clean_validate_optimize_id3dx_mesh(ID3DXMesh** mesh) {
		ID3DXBuffer* error_buffer = NULL;
		D3D9_VERIFY(::D3DXCreateBuffer(1024UL, &error_buffer));

		auto adjacency = std::unique_ptr<DWORD[]>(new DWORD[(*mesh)->GetNumFaces() * 3]);
		D3D9_VERIFY((*mesh)->GenerateAdjacency(0.0f, adjacency.get()));

		ID3DXMesh* clean_mesh = NULL;
		auto clean_adjacency = std::unique_ptr<DWORD[]>(new DWORD[(*mesh)->GetNumFaces() * 3]);
		D3D9_VERIFY(::D3DXCleanMesh(
			D3DXCLEAN_SIMPLIFICATION,
			*mesh,
			adjacency.get(),
			&clean_mesh,
			clean_adjacency.get(),
			&error_buffer));
		if (clean_mesh != NULL) {
			*mesh = clean_mesh;
			adjacency = std::move(clean_adjacency);
		}

		D3D9_VERIFY(::D3DXValidMesh(*mesh, adjacency.get(), &error_buffer));
		D3D9_VERIFY((*mesh)->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, adjacency.get(), NULL, NULL, NULL));

		d3d9_release_com_object(error_buffer);
	}

}