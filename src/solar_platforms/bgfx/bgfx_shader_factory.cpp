#include "bgfx_shader_factory.h"

#include "solar/utility/assert.h"
#include "solar/resources/resource_system.h"

namespace solar {

	bgfx_shader_factory::bgfx_shader_factory(resource_system& resource_system)
		: _resource_system(resource_system)
		, _is_setup(false) {
	}

	bgfx_shader_factory::~bgfx_shader_factory() {
		ASSERT(!_is_setup);
		ASSERT(_shaders.empty());
	}

	void bgfx_shader_factory::setup() {
		ASSERT(!_is_setup);
		_is_setup = true;
	}

	void bgfx_shader_factory::teardown() {
		remove_all_shaders();
		_is_setup = false;
	}

	void bgfx_shader_factory::remove_all_shaders() {
		_resource_system.end_watching_resources(this);
		_shaders.clear();
		_caching_context.increment();
	}

	shader* bgfx_shader_factory::get_shader(const std::string& id, const std::string& id_source_description) {
		ASSERT(_is_setup);

		auto iter = _shaders.find(id);
		if (iter != _shaders.end()) {
			return iter->second.get();
		}

		auto address = _resource_system.resolve_address("shader", "shaders", ".fxo", id.c_str(), id_source_description.c_str());
		bgfx_shader* new_shader = nullptr;
		if (!address.empty()) {
			new_shader = new bgfx_shader(address);
			_resource_system.begin_watching_resource(this, address, new_shader);
		}
		else {
			//todo - fallback
			new_shader = new bgfx_shader(resource_address());
		}
		_shaders[id] = std::unique_ptr<bgfx_shader>(new_shader);

		return new_shader;
	}

	const resource_factory_caching_context& bgfx_shader_factory::get_caching_context() const {
		return _caching_context;
	}

	void bgfx_shader_factory::on_file_changed(const std::string& path, void* data) {
		bgfx_shader* shader = reinterpret_cast<bgfx_shader*>(data);
		ASSERT(shader->get_resource_address().get_file_path() == path);
		//todo
	}

}