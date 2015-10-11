#include "keyboard_key.h"
#include "solar/utility/enum_helpers.h"

namespace solar {

	const char* to_string(keyboard_key key) {
		switch (key) {
			RETURN_ENUM_STRING(keyboard_key, invalid);

			RETURN_ENUM_STRING(keyboard_key, BACKSPACE);
			RETURN_ENUM_STRING(keyboard_key, DEL);
			RETURN_ENUM_STRING(keyboard_key, HOME);
			RETURN_ENUM_STRING(keyboard_key, END);
			RETURN_ENUM_STRING(keyboard_key, PAGE_DOWN);
			RETURN_ENUM_STRING(keyboard_key, PAGE_UP);
			RETURN_ENUM_STRING(keyboard_key, ENTER);
			RETURN_ENUM_STRING(keyboard_key, ESCAPE);
			RETURN_ENUM_STRING(keyboard_key, TAB);
			RETURN_ENUM_STRING(keyboard_key, ALT);
			RETURN_ENUM_STRING(keyboard_key, CTRL);
			RETURN_ENUM_STRING(keyboard_key, SHIFT);
			RETURN_ENUM_STRING(keyboard_key, SPACE);
			RETURN_ENUM_STRING(keyboard_key, LEFT);
			RETURN_ENUM_STRING(keyboard_key, RIGHT);
			RETURN_ENUM_STRING(keyboard_key, DOWN);
			RETURN_ENUM_STRING(keyboard_key, UP);
			RETURN_ENUM_STRING(keyboard_key, PAUSE);
			RETURN_ENUM_STRING(keyboard_key, INSERT);
			RETURN_ENUM_STRING(keyboard_key, HELP);
			RETURN_ENUM_STRING(keyboard_key, SEMI_COLON);
			RETURN_ENUM_STRING(keyboard_key, COMMA);
			RETURN_ENUM_STRING(keyboard_key, PERIOD);
			RETURN_ENUM_STRING(keyboard_key, FORWARD_SLASH);
			RETURN_ENUM_STRING(keyboard_key, BACK_SLASH);
			RETURN_ENUM_STRING(keyboard_key, TILDE);
			RETURN_ENUM_STRING(keyboard_key, OPEN_BRACKET);
			RETURN_ENUM_STRING(keyboard_key, CLOSE_BRACKET);
			RETURN_ENUM_STRING(keyboard_key, PLUS);
			RETURN_ENUM_STRING(keyboard_key, MINUS);
			RETURN_ENUM_STRING(keyboard_key, QUOTE);
			RETURN_ENUM_STRING(keyboard_key, PRINT_SCREEN);
			RETURN_ENUM_STRING(keyboard_key, SCROLL_LOCK);

			RETURN_ENUM_STRING(keyboard_key, A); 
			RETURN_ENUM_STRING(keyboard_key, B); 
			RETURN_ENUM_STRING(keyboard_key, C); 
			RETURN_ENUM_STRING(keyboard_key, D); 
			RETURN_ENUM_STRING(keyboard_key, E); 
			RETURN_ENUM_STRING(keyboard_key, F); 
			RETURN_ENUM_STRING(keyboard_key, G); 
			RETURN_ENUM_STRING(keyboard_key, H); 
			RETURN_ENUM_STRING(keyboard_key, I); 
			RETURN_ENUM_STRING(keyboard_key, J); 
			RETURN_ENUM_STRING(keyboard_key, K); 
			RETURN_ENUM_STRING(keyboard_key, L); 
			RETURN_ENUM_STRING(keyboard_key, M); 
			RETURN_ENUM_STRING(keyboard_key, N); 
			RETURN_ENUM_STRING(keyboard_key, O); 
			RETURN_ENUM_STRING(keyboard_key, P); 
			RETURN_ENUM_STRING(keyboard_key, Q); 
			RETURN_ENUM_STRING(keyboard_key, R); 
			RETURN_ENUM_STRING(keyboard_key, S); 
			RETURN_ENUM_STRING(keyboard_key, T); 
			RETURN_ENUM_STRING(keyboard_key, U); 
			RETURN_ENUM_STRING(keyboard_key, V); 
			RETURN_ENUM_STRING(keyboard_key, W); 
			RETURN_ENUM_STRING(keyboard_key, X); 
			RETURN_ENUM_STRING(keyboard_key, Y); 
			RETURN_ENUM_STRING(keyboard_key, Z);
			RETURN_ENUM_STRING(keyboard_key, N0); 
			RETURN_ENUM_STRING(keyboard_key, N1); 
			RETURN_ENUM_STRING(keyboard_key, N2); 
			RETURN_ENUM_STRING(keyboard_key, N3); 
			RETURN_ENUM_STRING(keyboard_key, N4); 
			RETURN_ENUM_STRING(keyboard_key, N5); 
			RETURN_ENUM_STRING(keyboard_key, N6); 
			RETURN_ENUM_STRING(keyboard_key, N7); 
			RETURN_ENUM_STRING(keyboard_key, N8); 
			RETURN_ENUM_STRING(keyboard_key, N9);
			RETURN_ENUM_STRING(keyboard_key, F1); 
			RETURN_ENUM_STRING(keyboard_key, F2); 
			RETURN_ENUM_STRING(keyboard_key, F3); 
			RETURN_ENUM_STRING(keyboard_key, F4); 
			RETURN_ENUM_STRING(keyboard_key, F5); 
			RETURN_ENUM_STRING(keyboard_key, F6); 
			RETURN_ENUM_STRING(keyboard_key, F7); 
			RETURN_ENUM_STRING(keyboard_key, F8); 
			RETURN_ENUM_STRING(keyboard_key, F9); 
			RETURN_ENUM_STRING(keyboard_key, F10); 
			RETURN_ENUM_STRING(keyboard_key, F11); 
			RETURN_ENUM_STRING(keyboard_key, F12);

			END_ENUM_STRINGS(key);
		}
	}

}