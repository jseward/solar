#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/shaders/shader_program_factory.h"
#include "solar/io/file_change_handler.h"
#include "d3d9_headers.h"
#include "d3d9_shader_program.h"
#include "d3d9_device_event_handler.h"

namespace solar {

	class d3d9_context;
	class resource_system;
	class resource_mapped_memory;

	class d3d9_shader_program_factory
		: public shader_program_factory
		, public file_change_handler
		, public d3d9_device_event_handler {

	private:
		static const char* FALLBACK_SHADER_CODE;

	private:
		d3d9_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _shader_program_caching_context;
		bool _is_debug_shaders_enabled;
		std::unordered_map<std::string, std::unique_ptr<d3d9_shader_program>> _shader_programs;
		std::vector<d3d9_shader_program*> _embedded_code_shader_programs;
		ID3DXEffectPool* _ID3DXEffectPool;
		std::unique_ptr<resource_mapped_memory> _resource_mapped_memory;

	public:
		d3d9_shader_program_factory(d3d9_context& context, resource_system& resource_system);
		virtual ~d3d9_shader_program_factory();

		void setup();
		void teardown();

		virtual shader_program* get_shader_program(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_shader_program_caching_context() const override;

		d3d9_shader_program* create_embedded_code_shader_program(const char* embedded_code);
		void release_embedded_code_shader_program(d3d9_shader_program* program);

		ID3DXEffectPool* get_ID3DXEffectPool();
		DWORD get_d3dxcreateeffect_flags() const;
		resource_system& get_resource_system();
		resource_mapped_memory& get_resource_mapped_memory();

	private:
		void remove_all_shader_programs();

		virtual void on_device_created(IDirect3DDevice9* device) override;
		virtual void on_device_released(IDirect3DDevice9* device) override;
		virtual void on_device_reset(IDirect3DDevice9* device) override;
		virtual void on_device_lost(IDirect3DDevice9* device) override;

		virtual void on_file_changed(const std::string& path, void* data) override;
	};

}