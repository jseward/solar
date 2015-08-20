#pragma once

#include <memory>
#include "solar/io/stream.h"
#include "solar/json/internal/rapidjson_headers.h"
#include "json_object.h"
#include "json_array.h"

namespace solar {

	typedef std::function<void(const std::string&)> json_document_error_handler;

	class json_document {
	private:
		std::unique_ptr<rapidjson::Document> _document;
		std::string _source_description;
		json_document_error_handler _error_handler;

	public:
		explicit json_document(stream& stream);
		json_document(stream& stream, json_document_error_handler error_handler);
		json_document(json_document&& rhs);
		json_document(const json_document&) = delete;
		json_document& operator=(const json_document&) = delete;

	private:
		void parse_stream(stream& stream);
		void default_error_handler(const std::string& msg);

	public:
		//http://stackoverflow.com/questions/13159739/how-to-disallow-temporaries
		//if json_document is created as a temporary then the internal rapdidjson
		//memory will be freed causing crashes when using either object or array.
		friend json_object convert_json_document_to_json_object(json_document& doc);
		friend json_array convert_json_document_to_json_array(json_document& doc);
	};

}
