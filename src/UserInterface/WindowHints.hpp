#pragma once

namespace RH {
namespace UI {

class WindowHints {

public:

	enum flag_enums {

		FULLSCREEN = (1 << 0),
		DECORATIONS = (1 << 1),
		CURSOR = (1 << 2),
		KEY_EVENTS = (1 << 3),
		CLICK_EVENTS = (1 << 4),
		MOUSEMOVE_EVENTS = (1 << 5),
		ONTOP = (1 << 6),
	};

private:

	int f;
	int x;
	int y;
	int w;
	int h;

public:

	WindowHints(int flags, int x, int y, int w, int h) :
			f(flags), x(x), y(y), w(w), h(h) {
	}

	int GetX() const {
		return x;
	}
	int GetY() const {
		return y;
	}
	int GetW() const {
		return w;
	}
	int GetH() const {
		return h;
	}
	int GetFlags() const {
		return f;
	}

	bool IsFullScreen() const {
		return !!(f & FULLSCREEN);
	}
	bool IsNotFullScreen() const {
		return !(f & FULLSCREEN);
	}
	bool IsDecorated() const {
		return !!(f & DECORATIONS);
	}
	bool IsNotDecorated() const {
		return !(f & DECORATIONS);
	}
	bool HasCursor() const {
		return !!(f & CURSOR);
	}
	bool DoesGenerateKeyEvents() const {
		return !!(f & KEY_EVENTS);
	}
	bool DoesGenerateClickEvents() const {
		return !!(f & CLICK_EVENTS);
	}
	bool DoesGenerateMouseMoveEvents() const {
		return !!(f & MOUSEMOVE_EVENTS);
	}
	bool IsOnTop() const {
		return !!(f & ONTOP);
	}

	WindowHints & SetX(int i) {
		x = i;
		return *this;
	}
	WindowHints & SetY(int i) {
		y = i;
		return *this;
	}
	WindowHints & SetW(int i) {
		w = i;
		return *this;
	}
	WindowHints & SetH(int i) {
		h = i;
		return *this;
	}
	WindowHints & SetFlags(int i) {
		f = i;
		return *this;
	}

	WindowHints & SetFullScreen(bool b) {
		if (b)
			f |= FULLSCREEN;
		else
			f &= ~FULLSCREEN;
		return *this;
	}
	WindowHints & SetDecorated(bool b) {
		if (b)
			f |= DECORATIONS;
		else
			f &= ~DECORATIONS;
		return *this;
	}
	WindowHints & SetCursor(bool b) {
		if (b)
			f |= CURSOR;
		else
			f &= ~CURSOR;
		return *this;
	}
	WindowHints & SetGenerateKeyEvents(bool b) {
		if (b)
			f |= KEY_EVENTS;
		else
			f &= ~KEY_EVENTS;
		return *this;
	}
	WindowHints & SetGenerateClickEvents(bool b) {
		if (b)
			f |= CLICK_EVENTS;
		else
			f &= ~CLICK_EVENTS;
		return *this;
	}
	WindowHints & SetGenerateMouseMoveEvents(bool b) {
		if (b)
			f |= MOUSEMOVE_EVENTS;
		else
			f &= ~MOUSEMOVE_EVENTS;
		return *this;
	}
	WindowHints & SetOnTop(bool b) {
		if (b)
			f |= ONTOP;
		else
			f &= ~ONTOP;
		return *this;
	}
};

}
}

