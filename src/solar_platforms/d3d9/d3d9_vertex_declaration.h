#pragma once

#include "d3d9_headers.h"
#include "solar/utility/assert.h"

namespace solar {

	class d3d9_vertex_declaration
	{
	private:
		D3DVERTEXELEMENT9 _elements[MAX_FVF_DECL_SIZE];
		int _next_element_index;

	public:
		d3d9_vertex_declaration()
			: _next_element_index(0) {
		}

		d3d9_vertex_declaration(const d3d9_vertex_declaration& rhs) = delete;
		d3d9_vertex_declaration& operator=(const d3d9_vertex_declaration& rhs) = delete;

		const D3DVERTEXELEMENT9* get_elements_begin() const {
			ASSERT(_next_element_index == -1);//add_end() called?
			return _elements;
		}

		unsigned int size() const {
			ASSERT(_next_element_index == -1);//add_end() called?
			return ::D3DXGetDeclVertexSize(_elements, 0);
		}

		void add_element(WORD stream, WORD offset, BYTE type, BYTE method, BYTE usage, BYTE usage_index) {
			D3DVERTEXELEMENT9* element = &_elements[_next_element_index];
			element->Stream = stream;
			element->Offset = offset;
			element->Type = type;
			element->Method = method;
			element->Usage = usage;
			element->UsageIndex = usage_index;
			_next_element_index += 1;
		}

		void add_end() {
			D3DVERTEXELEMENT9 end = D3DDECL_END();
			_elements[_next_element_index] = end;
			_next_element_index = -1;
		}
	};

}