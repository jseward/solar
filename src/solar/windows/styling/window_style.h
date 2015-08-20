#pragma once

#include <memory>
#include "solar/archiving/archivable.h"

namespace solar {

	class window_style : public archivable {
	public:
		virtual std::unique_ptr<window_style> clone() const = 0;
	};

}