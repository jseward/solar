#pragma once

#include "solar/json/internal/rapidjson_headers.h"
#include "solar/json/internal/rapidjson_stream.h"
#include "solar/utility/assert.h"
#include "json_value_builder.h"

namespace solar {

	template<typename AllocatorT>
	class json_array_builder : public json_value_builder<AllocatorT> {
	public:
		json_array_builder(AllocatorT& allocator)
			: json_value_builder(allocator, rapidjson::kArrayType) {
		}
		
		json_array_builder(const json_array_builder&) = delete;
		json_array_builder& operator=(const json_array_builder&) = delete;

		void reserve(size_t count) {
			get_value().Reserve(count, get_allocator());
		}

		void push_back_object(json_value_builder<AllocatorT>& object) {
			ASSERT(object.get_value().GetType() == rapidjson::kObjectType);
			get_value().PushBack(object.get_value(), get_allocator());
		}

		void write_to_stream(stream& stream) {
			rapidjson_stream rj_stream(stream);
			rapidjson::PrettyWriter<rapidjson_stream> writer(rj_stream);
			get_value().Accept(writer);
		}
	};

}
