#include "bgfx_shader_program_factory.h"

#include "solar/utility/assert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/resources/resource_system.h"

namespace solar {

	bgfx_shader_program_factory::bgfx_shader_program_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	bgfx_shader_program_factory::~bgfx_shader_program_factory() {
		ASSERT(!_is_setup);
		ASSERT(_shader_programs.empty());
	}

	void bgfx_shader_program_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void bgfx_shader_program_factory::teardown() {
		remove_all_shaders();
		_is_setup = false;
	}

	void bgfx_shader_program_factory::remove_all_shaders() {
		_resource_system.end_watching_resources(this);
		_shader_programs.clear();
		_shader_program_caching_context.increment();
	}

	shader_program* bgfx_shader_program_factory::get_shader_program(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _shader_programs.find(id);
		if (iter != _shader_programs.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address("shader_program", "shaders", ".shader_program", id.c_str(), id_source_description.c_str());
		bgfx_shader_program* new_program = new bgfx_shader_program(*this, address);
		if (!address.empty()) {
			new_program->load();
			_resource_system.begin_watching_resource(this, address, new_program);
		}
		_shader_programs[id] = std::unique_ptr<bgfx_shader_program>(new_program);
		return new_program;
	}

	const resource_factory_caching_context& bgfx_shader_program_factory::get_shader_program_caching_context() const {
		return _shader_program_caching_context;
	}

	resource_system& bgfx_shader_program_factory::get_resource_system() {
		return _resource_system;
	}

	void bgfx_shader_program_factory::on_file_changed(const std::string& path, void* data) {
		bgfx_shader_program* program = reinterpret_cast<bgfx_shader_program*>(data);
		ASSERT(program->get_resource_address().get_file_path() == path);
		program->load();
	}

}