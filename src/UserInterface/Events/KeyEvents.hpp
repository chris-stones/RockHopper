
#pragma once

namespace RH { namespace UI { namespace Events {

enum keys {
	KEY_UNKNOWN,
	KEY_ESCAPE,
	KEY_SPACE,
	KEY_RETURN,
	KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,
	KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,
	KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
};

class KeyPressedEvent {
	keys k;
public:
	KeyPressedEvent(keys k)
		:	k(k)
	{}
	keys GetKey() const {
		return k;
	}
	bool operator < (const KeyPressedEvent & that) const {
		return this->k < that.k;
	}
};

class KeyReleasedEvent {
	keys k;
public:
	KeyReleasedEvent(keys k)
		:	k(k)
	{}
	keys GetKey() const {
		return k;
	}
	bool operator < (const KeyReleasedEvent & that) const {
		return this->k < that.k;
	}
};

}}}
