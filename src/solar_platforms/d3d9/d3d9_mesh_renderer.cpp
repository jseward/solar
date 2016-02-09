#include "d3d9_mesh_renderer.h"

#include "solar/utility/assert.h"
#include "solar/utility/verify.h"
#include "solar/utility/unused_parameter.h"
#include "solar/rendering/shaders/shader_program.h"
#include "solar/rendering/cameras/camera.h"
#include "d3d9_release_com_object.h"
#include "d3d9_mesh.h"
#include "d3d9_mesh_material.h"
#include "d3d9_mesh_vertex_declaration.h"
#include "d3d9_verify.h"
#include "d3d9_context.h"

namespace solar {

	const char* d3d9_mesh_renderer::shader_param_names::WORLD_TRANSFORM = "_world_transform";
	const char* d3d9_mesh_renderer::shader_param_names::WORLD_VIEW_PROJECTION_TRANSFORM = "_world_view_projection_transform";
	const char* d3d9_mesh_renderer::shader_param_names::DIFFUSE_MAP = "_diffuse_texture";
	const char* d3d9_mesh_renderer::shader_param_names::NORMAL_MAP = "_normal_texture";

	d3d9_mesh_renderer::d3d9_mesh_renderer(d3d9_context& context) 
		: _context(context)
		, _vertex_declaration(nullptr)
		, _is_rendering(false)
		, _shader_program(nullptr)
		, _camera(nullptr) {
	}

	d3d9_mesh_renderer::~d3d9_mesh_renderer() {
		ASSERT(_vertex_declaration == nullptr);
		ASSERT(!_is_rendering);
		ASSERT(_shader_program == nullptr);
		ASSERT(_camera == nullptr);
	}

	void d3d9_mesh_renderer::setup() {
		_context.add_device_event_handler(this);
	}

	void d3d9_mesh_renderer::teardown() {
		_context.remove_device_event_handler(this);
	}

	void d3d9_mesh_renderer::begin_rendering(const camera& camera, shader_program& shader_program) {
		ASSERT(!_is_rendering);
		_is_rendering = true;
		_shader_program = &shader_program;
		_camera = &camera;
		D3D9_VERIFY(_context.get_device()->SetVertexDeclaration(_vertex_declaration));
		_shader_program->start();
	}

	void d3d9_mesh_renderer::end_rendering() {
		_shader_program->stop();
		_is_rendering = false;
		_shader_program = nullptr;
		_camera = nullptr;
	}

	void d3d9_mesh_renderer::begin_rendering_sub_mesh(const mesh& generic_mesh, int sub_mesh_index, const mat44& world_transform) {
		ASSERT(_is_rendering);

		const d3d9_mesh& mesh = static_cast<const d3d9_mesh&>(generic_mesh);
		D3D9_VERIFY(_context.get_device()->SetStreamSource(0U, mesh._vertex_buffer, 0U, mesh._id3dx_mesh->GetNumBytesPerVertex()));
		D3D9_VERIFY(_context.get_device()->SetIndices(mesh._index_buffer));

		ASSERT(sub_mesh_index >= 0);
		ASSERT(sub_mesh_index < static_cast<int>(mesh._attribute_table_size));
		auto material_index = mesh._attribute_table[sub_mesh_index].AttribId;
		VERIFY(_shader_program->set_texture(shader_param_names::DIFFUSE_MAP, mesh._materials.at(material_index)->_diffuse_map.get_texture()));
		VERIFY(_shader_program->set_texture(shader_param_names::NORMAL_MAP, mesh._materials.at(material_index)->_normal_map.get_texture()));

		VERIFY(_shader_program->set_mat44(shader_param_names::WORLD_TRANSFORM, world_transform));
		VERIFY(_shader_program->set_mat44(shader_param_names::WORLD_VIEW_PROJECTION_TRANSFORM, _camera->build_world_view_projection_transform(world_transform)));

		_shader_program->commit_param_changes();
	}

	void d3d9_mesh_renderer::end_rendering_sub_mesh(const mesh& mesh, int sub_mesh_index) {		
		UNUSED_PARAMETER(mesh);
		UNUSED_PARAMETER(sub_mesh_index);
	}

	void d3d9_mesh_renderer::render_sub_mesh(const mesh& generic_mesh, int sub_mesh_index) {
		const d3d9_mesh& mesh = static_cast<const d3d9_mesh&>(generic_mesh);
		ASSERT(sub_mesh_index >= 0);
		ASSERT(sub_mesh_index < static_cast<int>(mesh._attribute_table_size));
		D3D9_VERIFY(_context.get_device()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			mesh._attribute_table[sub_mesh_index].VertexStart,
			mesh._attribute_table[sub_mesh_index].VertexCount,
			mesh._attribute_table[sub_mesh_index].FaceStart * 3,
			mesh._attribute_table[sub_mesh_index].FaceCount));
	}

	void d3d9_mesh_renderer::on_device_created(IDirect3DDevice9* device) {
		ASSERT(_vertex_declaration == nullptr);
		D3D9_VERIFY(device->CreateVertexDeclaration(d3d9_mesh_vertex_declaration().get_elements_begin(), &_vertex_declaration));
	}

	void d3d9_mesh_renderer::on_device_released(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
		d3d9_release_com_object(_vertex_declaration);
	}

	void d3d9_mesh_renderer::on_device_reset(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

	void d3d9_mesh_renderer::on_device_lost(IDirect3DDevice9* device) {
		UNUSED_PARAMETER(device);
	}

}