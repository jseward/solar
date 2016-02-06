#pragma once

#include <deque>

namespace solar {

	class archive_context_stack {
	public:
		enum class frame_type {
			OBJECT_MEMBER,
			ARRAY_ELEMENT
		};

		class frame {
		public:
			frame_type _type;
			const char* _member_name;
			unsigned int _array_index;

		public:
			frame();
			bool is_object_member() const { return _type == frame_type::OBJECT_MEMBER; }
			bool is_array_element() const { return _type == frame_type::ARRAY_ELEMENT; }
		};

	private:
		std::deque<frame> _frames;

	public:
		archive_context_stack();
		
		void push_object_member(const char* name);
		void push_array_element(unsigned int index);
		frame pop();

		std::string to_string() const;
	};

}