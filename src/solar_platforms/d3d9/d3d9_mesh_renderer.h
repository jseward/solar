#pragma once

#include "solar/rendering/meshes/mesh_renderer.h"
#include "d3d9_device_event_handler.h"

namespace solar {
	
	class d3d9_context;

	class d3d9_mesh_renderer 
		: public mesh_renderer 
		, public d3d9_device_event_handler {

	private:
		struct shader_param_names {
			static const char* WORLD_TRANSFORM;
			static const char* WORLD_VIEW_PROJECTION_TRANSFORM;
			static const char* DIFFUSE_MAP;
			static const char* NORMAL_MAP;
		};

	private:
		d3d9_context& _context;
		IDirect3DVertexDeclaration9* _vertex_declaration;
		bool _is_rendering;
		bool _is_rendering_mesh;
		shader* _shader;
		const camera* _camera;

	public:
		d3d9_mesh_renderer(d3d9_context& context);
		virtual ~d3d9_mesh_renderer();

		void setup();
		void teardown();

		virtual void begin_rendering(const camera& camera, shader& shader) override;
		virtual void end_rendering() override;
		virtual void begin_rendering_sub_mesh(const mesh& mesh, int sub_mesh_index, const mat44& world_transform) override;
		virtual void end_rendering_sub_mesh(const mesh& mesh, int sub_mesh_index) override;
		virtual void render_sub_mesh(const mesh& mesh, int sub_mesh_index) override;

	private:
		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;
	};

}