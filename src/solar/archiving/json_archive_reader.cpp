#include "json_archive_reader.h"
#include "solar/utility/alert.h"
#include "solar/strings/string_build.h"
#include "archivable.h"
#include "single_value_archivable.h"

namespace solar {

	json_archive_reader::json_archive_reader(stream& stream)
		: _stream(stream)
		, _document(stream, [this](const std::string& msg) { raise_error(msg); })
		, _current_object(new json_object(convert_json_document_to_json_object(_document))) {

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_current_object->set_should_track_used_values();
		#endif
	}

	json_archive_reader::~json_archive_reader() {
		
		//note: do nothing involving stream here. no guarantee it still exists and is probably unowned memory.
		//ex. do not raise errors!

		delete _current_object;
	}

	void json_archive_reader::attempt_raise_error_for_unused_values() const {
		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_current_object->raise_error_for_unused_values("root");
		#endif
	}

	std::string json_archive_reader::get_source_description() const {
		return _stream.get_description();
	}

	void json_archive_reader::raise_error(const std::string& error_message) {
		ALERT("json_archive_reader error : {}\nstream : {}", error_message, _stream.get_description());
	}

	void json_archive_reader::read_object(const char* name, archivable& value) {
		json_object o;
		if (!_current_object->try_get_object(o, name)) {
			raise_error(build_string("object not found : '{}'", name));
		}
		else {
			temp_swap_current_object swap(this, name, &o);
			value.read_from_archive(*this);
		}
	}

	void json_archive_reader::read_objects(const char* name, std::function<void(unsigned int)> handle_size_func, std::function<void(archive_reader&, unsigned int)> read_object_func) {
		json_array a;
		if (!_current_object->try_get_array(a, name)) {
			raise_error(build_string("array not found : '{}'", name));
		}
		else {
			handle_size_func(a.size());
			for (unsigned int i = 0; i < a.size(); ++i) {
				auto i_object = a.get_object(i);
				temp_swap_current_object swap(this, name, &i_object);
				read_object_func(*this, i);
			}
		}
	}

	void json_archive_reader::read_bool(const char* name, bool& value) {
		value = false;
		if (!_current_object->try_get_bool(value, name)) {
			raise_error(build_string("bool not found : '{}'", name));
		}
	}

	void json_archive_reader::read_ushort(const char* name, unsigned short& value) {
		value = 0;
		if (!_current_object->try_get_ushort(value, name)) {
			raise_error(build_string("ushort not found : '{}'", name));
		}
	}

	void json_archive_reader::read_ushorts(const char* name, unsigned short* begin, unsigned int count) {
		json_array a;
		if (try_get_array_of_size(a, name, count)) {
			for (unsigned int i = 0; i < count; ++i) {
				begin[i] = a.get_ushort(i);
			}
		}
	}

	void json_archive_reader::read_int(const char* name, int& value) {
		if (!_current_object->try_get_int(value, name)) {
			raise_error(build_string("int not found : '{}'", name));
		}
	}

	void json_archive_reader::read_ints(const char* name, int* begin, unsigned int count) {
		json_array a;
		if (try_get_array_of_size(a, name, count)) {
			for (unsigned int i = 0; i < count; ++i) {
				begin[i] = a.get_int(i);
			}
		}
	}

	void json_archive_reader::read_uint(const char* name, unsigned int& value) {
		if (!_current_object->try_get_uint(value, name)) {
			raise_error(build_string("uint not found : '{}'", name));
		}
	}

	void json_archive_reader::read_float(const char* name, float& value) {
		if (!_current_object->try_get_float(value, name)) {
			raise_error(build_string("float not found : '{}'", name));
		}
	}

	void json_archive_reader::read_floats(const char* name, float* begin, unsigned int count) {
		json_array a;
		if (try_get_array_of_size(a, name, count)) {
			for (unsigned int i = 0; i < count; ++i) {
				begin[i] = a.get_float(i);
			}
		}
	}

	void json_archive_reader::read_string(const char* name, std::string& value) {
		if (!_current_object->try_get_string(value, name)) {
			raise_error(build_string("string not found : '{}'", name));
		}
	}

	bool json_archive_reader::try_get_array_of_size(json_array& out, const char* name, unsigned int required_size) {
		if (!_current_object->try_get_array(out, name)) {
			raise_error(build_string("array not found : '{}'", name));
			return false;
		}

		if (out.size() != required_size) {
			raise_error(build_string("array size != {} : '{}'", required_size, name));
			return false;
		}

		return true;
	}

	json_archive_reader::temp_swap_current_object::temp_swap_current_object(json_archive_reader* reader, const char* name, json_object* new_object) 
		: _reader(reader)
		, _name(name)
		, _old_object(reader->_current_object)
		, _new_object(new_object) {
		
		reader->_current_object = _new_object;

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_new_object->set_should_track_used_values();
		#endif

	}

	json_archive_reader::temp_swap_current_object::~temp_swap_current_object() {
		_reader->_current_object = _old_object;

		#ifndef SOLAR__JSON_ARCHIVE_READER_NO_ALERT_UNUSED_VALUES
		_new_object->raise_error_for_unused_values(_name);
		#endif
	}

}
