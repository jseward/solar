#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "text.h"

namespace solar {

	class stream;

	class text_map : public std::unordered_map<std::string, std::unique_ptr<text>> {
	public:
		void load_from_stream(stream& s);
		void save_to_stream(stream& s);
	};

}