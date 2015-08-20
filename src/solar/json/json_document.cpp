#include "json_document.h"
#include "solar/strings/string_build.h"
#include "solar/utility/assert.h"
#include "solar/utility/alert.h"
#include "solar/json/internal/rapidjson_stream.h"
#include "solar/json/internal/rapidjson_headers.h"

namespace solar {

	json_document::json_document(stream& stream) 
		: _source_description(stream.get_description())
		, _error_handler([&](const std::string& msg) { default_error_handler(msg); }) {

		parse_stream(stream);
	}

	json_document::json_document(stream& stream, json_document_error_handler error_handler)
		: _source_description(stream.get_description())
		, _error_handler(error_handler) {

		parse_stream(stream);
	}

	json_document::json_document(json_document&& rhs) 
		: _document(std::move(rhs._document))
		, _error_handler(rhs._error_handler) {
	}

	void json_document::parse_stream(stream& stream) {
		auto document_stream = rapidjson_stream(stream);
		_document = std::make_unique<rapidjson::Document>();
		_document->ParseStream<0>(document_stream);
		if (_document->HasParseError()) {
			_error_handler(build_string("parsing stream failed : {} ({})", rapidjson::GetParseError_En(_document->GetParseError()), _document->GetErrorOffset()));
		}
	}

	void json_document::default_error_handler(const std::string& msg) {
		ALERT("json error in '{}'\n{}", _source_description, msg);
	}

	json_object convert_json_document_to_json_object(json_document& doc) {
		if (!doc._document->IsObject()) {
			doc._error_handler("document root is not an object");
			return json_object();
		}
		return json_object(static_cast<rapidjson::Value&>(*doc._document.get()), doc._error_handler);
	}

	json_array convert_json_document_to_json_array(json_document& doc) {
		if (!doc._document->IsArray()) {
			doc._error_handler("document root is not an array");
			return json_array();
		}
		return json_array(static_cast<rapidjson::Value&>(*doc._document.get()), doc._error_handler);
	}

}
