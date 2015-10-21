#pragma once

#include "solar/io/clipboard.h"
#include "win32_headers.h"

namespace solar {

	class win32_clipboard : public clipboard {
	private:
		HWND _hwnd;

	public:
		win32_clipboard();
		virtual ~win32_clipboard();

		void setup(HWND hwnd);

		virtual void set_clipboard_text(const std::wstring& text) override;
		virtual std::wstring get_clipboard_text() override;
	};

}