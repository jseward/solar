#pragma once

#include <memory>
#include "solar/json/internal/rapidjson_headers.h"

namespace solar {

	//needed as a base for both json_array_builder and json_object_builder so both can add eachother.

	template<typename AllocatorT>
	class json_value_builder {
	private:
		AllocatorT& _allocator;
		std::unique_ptr<rapidjson::Value> _value;

	public:
		json_value_builder(AllocatorT& allocator, rapidjson::Type type)
			: _allocator(allocator)
			, _value(std::make_unique<rapidjson::Value>(type)) {
		}

		json_value_builder(const json_value_builder&) = delete;
		json_value_builder& operator=(const json_value_builder&) = delete;

		AllocatorT& get_allocator() {
			return _allocator;
		}

		rapidjson::Value& get_value() {
			ASSERT(_value != nullptr);
			return *(_value.get());
		}
	};

}
