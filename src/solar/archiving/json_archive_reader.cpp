#include "json_archive_reader.h"
#include "solar/utility/alert.h"
#include "solar/utility/unused_parameter.h"
#include "solar/strings/string_build.h"
#include "solar/colors/color.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {

	json_archive_reader::json_archive_reader(stream& stream)
		: _stream(stream)
		, _document(stream, [this](const std::string& msg) { raise_error(msg); })
		, _root_object(convert_json_document_to_json_object(_document))
		, _current_object(nullptr) 
		, _current_array(nullptr)
		, _context_stack() {

		_current_object = &_root_object;

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_current_object->set_should_track_used_values();
		#endif
	}

	json_archive_reader::~json_archive_reader() {
		//NOTE: do nothing involving stream here. no guarantee it still exists and is probably unowned memory.
		//ex. do not raise errors!
	}

	void json_archive_reader::attempt_raise_error_for_unused_values() const {
		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_current_object->raise_error_for_unused_values();
		#endif
	}

	std::string json_archive_reader::get_source_description() const {
		return _stream.get_description();
	}

	void json_archive_reader::raise_error(const std::string& error_message) const {
		ALERT("json_archive_reader error : {}\nstream : {}\ncontext : {}", error_message, _stream.get_description(), _context_stack.to_string());
	}

	unsigned int json_archive_reader::get_read_position() const {
		return 0; //not supported
	}

	void json_archive_reader::read_name(const char* name) {
		_context_stack.push_object_member(name);
	}

	void json_archive_reader::read_array(
		std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
		std::function<void(archive_reader&, unsigned int)> read_element_func) {

		//NOTE: has to handle read_array being called recursively (array of arrays)
		auto frame = _context_stack.pop();
		if (frame.is_array_element()) {
			ASSERT(_current_array != nullptr);
			json_array a = _current_array->get_array(frame._array_index);
			read_array_internal(handle_size_func, read_element_func, a);
		}
		else {
			ASSERT(frame.is_object_member());
			ASSERT(_current_object != nullptr);
			json_array a = _current_object->get_array(frame._member_name);
			read_array_internal(handle_size_func, read_element_func, a);
		}
	}

	void json_archive_reader::read_array_internal(
		std::function<bool(archive_reader&, unsigned int)> handle_size_func, 
		std::function<void(archive_reader&, unsigned int)> read_element_func, json_array& new_array) {

		json_array* old_array = _current_array;
		_current_array = &new_array;

		auto size = _current_array->size();
		if (handle_size_func(*this, size)) {
			for (unsigned int i = 0; i < size; ++i) {
				_context_stack.push_array_element(i);
				read_element_func(*this, i);
			}
		}

		_current_array = old_array;
	}

	void json_archive_reader::read_object(std::function<void(archive_reader&)> read_object_func) {
		auto frame = _context_stack.pop();
		if (frame.is_array_element()) {
			ASSERT(_current_array != nullptr);
			json_object o = _current_array->get_object(frame._array_index);
			read_object_internal(read_object_func, o);
		}
		else {
			ASSERT(frame.is_object_member());
			ASSERT(_current_object != nullptr);
			json_object o = _current_object->get_object(frame._member_name);
			read_object_internal(read_object_func, o);
		}
	}

	void json_archive_reader::read_object_internal(
		std::function<void(archive_reader&)> read_object_func, 
		json_object& new_object) {

		json_object* old_object = _current_object;
		_current_object = &new_object;

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		new_object.set_should_track_used_values();
		#endif

		read_object_func(*this);

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		new_object.raise_error_for_unused_values();
		#endif

		_current_object = old_object;
	}


	void json_archive_reader::read_bool(bool& value) {
		read_atomic_value<bool>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_bool(i); },
			[](const json_object& o, const char* n) { return o.get_bool(n); });
	}

	void json_archive_reader::read_uint16(uint16_t& value) {
		read_atomic_value<uint16_t>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_uint16(i); },
			[](const json_object& o, const char* n) { return o.get_uint16(n); });
	}

	void json_archive_reader::read_int(int& value, const archive_int_compression& compression) {
		UNUSED_PARAMETER(compression);
		read_atomic_value<int>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_int(i); },
			[](const json_object& o, const char* n) { return o.get_int(n); });
	}

	void json_archive_reader::read_int64(int64_t& value) {
		read_atomic_value<int64_t>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_int64(i); },
			[](const json_object& o, const char* n) { return o.get_int64(n); });
	}

	void json_archive_reader::read_uint(unsigned int& value) {
		read_atomic_value<unsigned int>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_uint(i); },
			[](const json_object& o, const char* n) { return o.get_uint(n); });
	}

	void json_archive_reader::read_float(float& value) {
		read_atomic_value<float>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_float(i); },
			[](const json_object& o, const char* n) { return o.get_float(n); });
	}

	void json_archive_reader::read_string(std::string& value) {
		read_atomic_value<std::string>(
			value,
			[](const json_array& a, unsigned int i) { return a.get_string(i); },
			[](const json_object& o, const char* n) { return o.get_string(n); });
	}

	void json_archive_reader::read_color(color& value) {
		std::string s;
		read_string(s);
		if (!s.empty()) {
			if (!try_make_color_from_string(value, s.c_str())) {
				raise_error(build_string("color could not be parsed from '{}'", s.c_str()));
			}
		}
	}

}
