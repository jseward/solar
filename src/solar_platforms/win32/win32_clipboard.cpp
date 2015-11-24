#include "win32_clipboard.h"

#include "solar/utility/verify.h"
#include "solar/utility/trace.h"
#include "solar/strings/string_helpers.h"
#include "win32_error.h"

namespace solar {

	win32_clipboard::win32_clipboard() 
		: _hwnd(nullptr) {
	}

	win32_clipboard::~win32_clipboard() {
	}

	void win32_clipboard::setup(HWND hwnd) {
		_hwnd = hwnd;
	}

	void win32_clipboard::set_clipboard_text(const std::wstring& text) {
		IF_VERIFY(_hwnd != nullptr) {
			if (::OpenClipboard(_hwnd) == FALSE) {
				TRACE("WARNING : OpenClipboard() failed. {}", get_last_win32_error_message());
			}
			else {
				::EmptyClipboard();

				if (!text.empty()) {
					size_t memory_size = (text.length() + 1) * sizeof(wchar_t);
					HGLOBAL memory = ::GlobalAlloc(GMEM_MOVEABLE, memory_size);
					if (memory == nullptr) {
						TRACE("WARNING : Clipboard GlobalAlloc() failed.");
					}
					else {
						wchar_t* memory_as_wchar = reinterpret_cast<wchar_t*>(::GlobalLock(memory));
						if (memory_as_wchar == nullptr) {
							TRACE("WARNING : Clipboard GlobalLock() failed.");
						}
						else {
							::memcpy_s(memory_as_wchar, memory_size, text.c_str(), text.length() * sizeof(wchar_t));
							memory_as_wchar[text.length()] = L'\0';
							::GlobalUnlock(memory);
							::SetClipboardData(CF_UNICODETEXT, memory);
						}
					}
				}

				::CloseClipboard();
			}
		}
	}

	std::wstring win32_clipboard::get_clipboard_text() {
		std::wstring text;

		IF_VERIFY(_hwnd != nullptr) {
			if (::OpenClipboard(_hwnd) == FALSE) {
				TRACE("WARNING : OpenClipboard() failed. {}", get_last_win32_error_message());
			}
			else {
				HGLOBAL memory = ::GetClipboardData(CF_UNICODETEXT);
				if (memory == nullptr) {
					if (::GetLastError() != 0) {
						TRACE("WARNING : GetClipboardData() failed. {}", get_last_win32_error_message());
					}
				}
				else {
					const wchar_t* memory_as_wchar = reinterpret_cast<const wchar_t*>(::GlobalLock(memory));
					if (memory_as_wchar == nullptr) {
						TRACE("WARNING : Clipboard GlobalLock() failed.");
					}
					else {
						text = memory_as_wchar;
						::GlobalUnlock(memory);
					}
				}

				::CloseClipboard();
			}
		}

		return text;
	}

}