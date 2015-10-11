#pragma once

namespace solar {

	enum class keyboard_key {
		invalid = -1,

		BACKSPACE,
		DEL, //DELETE #define by win32
		HOME,
		END,
		PAGE_DOWN,
		PAGE_UP,
		ENTER,
		ESCAPE,
		TAB,
		ALT,
		CTRL,
		SHIFT,
		SPACE,
		LEFT,
		RIGHT,
		DOWN,
		UP,
		PAUSE,
		INSERT,
		HELP,
		SEMI_COLON,
		COMMA,
		PERIOD,
		FORWARD_SLASH,
		BACK_SLASH,
		TILDE,
		OPEN_BRACKET,
		CLOSE_BRACKET,
		PLUS,
		MINUS,
		QUOTE,
		PRINT_SCREEN,
		SCROLL_LOCK,

		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	};

	extern const char* to_string(keyboard_key key);

}