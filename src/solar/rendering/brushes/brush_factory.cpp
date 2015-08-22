#include "brush_factory.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/resources/resource_system.h"
#include "solar/resources/resource_id_manifest.h"
#include "brush_set.h"
#include "brush.h"

namespace solar {

	brush_factory::brush_factory(resource_system& resource_system) 
		: _resource_system(resource_system) {
	}

	void brush_factory::setup() {
		ASSERT(_brush_sets.empty());

		resource_id_manifest manifest;
		manifest.load(_resource_system, "brush_manifest", "brushes", ".brush_manifest", "_brush_manifest", "brush_factory");

		for (auto id : manifest.get_ids()) {
			auto address = _resource_system.resolve_address("brush", "brushes", ".brush", id.c_str(), manifest.get_address().to_string().c_str());
			_brush_sets.push_back(std::make_unique<brush_set>(address));
		}

		for (auto& brush_set : _brush_sets) {
			brush_set->load(_resource_system);
		}

		_caching_context.increment();
	}

	void brush_factory::teardown() {
	}

	const resource_factory_caching_context& brush_factory::get_caching_context() const {
		return _caching_context;
	}

	brush* brush_factory::get_brush(const std::string& id, const std::string& id_source_description) {
		for (auto& brush_set : _brush_sets) {
			brush* brush = brush_set->get_brush_if_exists(id);
			if (brush != nullptr) {
				return brush;
			}
		}

		ALERT("brush not found.\n\nid : {0}\nid_source_description : {1}", id, id_source_description);
		if (_empty_brush == nullptr) {
			_empty_brush = std::make_unique<brush>("");
		}
		return _empty_brush.get();
	}

}