#include "archive_context_stack.h"

#include <sstream>

#include "solar/utility/assert.h"

namespace solar {

	archive_context_stack::frame::frame()
		: _type(frame_type::OBJECT_MEMBER)
		, _member_name("")
		, _array_index(0) {
	}

	archive_context_stack::archive_context_stack() {
		ASSERT(_frames.empty());
	}
	
	void archive_context_stack::push_object_member(const char* name) {
		frame new_frame;
		new_frame._type = frame_type::OBJECT_MEMBER;
		new_frame._member_name = name;
		_frames.push_back(new_frame);
	}

	void archive_context_stack::push_array_element(unsigned int index) {
		frame new_frame;
		new_frame._type = frame_type::ARRAY_ELEMENT;
		new_frame._array_index = index;
		_frames.push_back(new_frame);
	}

	archive_context_stack::frame archive_context_stack::pop() {
		auto frame = _frames.back();
		_frames.pop_back();
		return frame;
	}

	std::string archive_context_stack::to_string() const {
		std::stringstream ss;
		ss << "root";
		for (const auto& frame : _frames) {
			if (frame._type == frame_type::OBJECT_MEMBER) {
				ss << "." << frame._member_name;
			}
			else {
				ss << "[" << frame._array_index << "]";
			}
		}
		return ss.str();
	}

}