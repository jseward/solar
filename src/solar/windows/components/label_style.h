#pragma once

#include "solar/utility/property.h"
#include "solar/windows/layout/window_layout.h"
#include "solar/windows/styling/window_style.h"
#include "solar/windows/styling/window_style_id.h"

namespace solar {

	class label_style : public window_style {
	public:
		static const char* TYPE_NAME;

	public:

	public:
		virtual std::unique_ptr<window_style> clone() const override;
		virtual void read_from_archive(archive_reader& reader) override;
		virtual void write_to_archive(archive_writer& writer) const override;
	};

	typedef window_style_id<label_style> label_style_id;

}
