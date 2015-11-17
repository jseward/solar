#pragma once

#include <unordered_map>
#include <memory>
#include "solar/rendering/shaders/shader_program_factory.h"
#include "solar/io/file_change_handler.h"
#include "bgfx_shader_program.h"
#include "bgfx_shader_embedded_code.h"

namespace solar {

	class bgfx_context;
	class resource_system;
	class resource_mapped_memory;

	class bgfx_shader_program_factory 
		: public shader_program_factory
		, public file_change_handler {

	private:
		bgfx_context& _context;
		resource_system& _resource_system;
		bool _is_setup;
		resource_factory_caching_context _shader_program_caching_context;
		std::unordered_map<std::string, std::unique_ptr<bgfx_shader_program>> _shader_programs;
		std::vector<bgfx_shader_program*> _embeded_code_shader_programs;

	public:
		bgfx_shader_program_factory(bgfx_context& context, resource_system& resource_system);
		virtual ~bgfx_shader_program_factory();

		void setup();
		void teardown();

		virtual shader_program* get_shader_program(const std::string& id, const std::string& id_source_description) override;
		virtual const resource_factory_caching_context& get_shader_program_caching_context() const override;

		bgfx_shader_program* create_embeded_code_shader_program(bgfx_shader_embedded_code vs_embedded_code, bgfx_shader_embedded_code fs_embedded_code);
		void release_embeded_code_shader_program(bgfx_shader_program* program);

		bgfx_context& get_bgfx_context();
		resource_system& get_resource_system();

	private:
		void remove_all_shaders();
		
		virtual void on_file_changed(const std::string& path, void* data) override;
	};

}