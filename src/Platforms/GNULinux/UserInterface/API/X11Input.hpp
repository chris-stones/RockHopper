
#pragma once

class Input_ {

	Display * dpy;
	Window window;
	long xEventsMask;

	static RH::Libs::EventDispatcher::EventPublicationManager eventPublicationManager;

	RH::UI::Events::keys ToKey(XKeyEvent * xkeyevent) {

		KeySym ks = XLookupKeysym(xkeyevent, 0);

		switch( ks ) {
			default: break;
			case XK_Escape: return RH::UI::Events::KEY_ESCAPE;
			case XK_space:  return RH::UI::Events::KEY_SPACE;
			case XK_Return: return RH::UI::Events::KEY_RETURN;
			case XK_0:	return RH::UI::Events::KEY_0;
			case XK_1:	return RH::UI::Events::KEY_1;
			case XK_2:	return RH::UI::Events::KEY_2;
			case XK_3:	return RH::UI::Events::KEY_3;
			case XK_4:	return RH::UI::Events::KEY_4;
			case XK_5:	return RH::UI::Events::KEY_5;
			case XK_6:	return RH::UI::Events::KEY_6;
			case XK_7:	return RH::UI::Events::KEY_7;
			case XK_8:	return RH::UI::Events::KEY_8;
			case XK_9:	return RH::UI::Events::KEY_9;
			case XK_a:	return RH::UI::Events::KEY_A;
			case XK_b:	return RH::UI::Events::KEY_B;
			case XK_c:	return RH::UI::Events::KEY_C;
			case XK_d:	return RH::UI::Events::KEY_D;
			case XK_e:	return RH::UI::Events::KEY_E;
			case XK_f:	return RH::UI::Events::KEY_F;
			case XK_g:	return RH::UI::Events::KEY_G;
			case XK_h:	return RH::UI::Events::KEY_H;
			case XK_i:	return RH::UI::Events::KEY_I;
			case XK_j:	return RH::UI::Events::KEY_J;
			case XK_k:	return RH::UI::Events::KEY_K;
			case XK_l:	return RH::UI::Events::KEY_L;
			case XK_m:	return RH::UI::Events::KEY_M;
			case XK_n:	return RH::UI::Events::KEY_N;
			case XK_o:	return RH::UI::Events::KEY_O;
			case XK_p:	return RH::UI::Events::KEY_P;
			case XK_q:	return RH::UI::Events::KEY_Q;
			case XK_r:	return RH::UI::Events::KEY_R;
			case XK_s:	return RH::UI::Events::KEY_S;
			case XK_t:	return RH::UI::Events::KEY_T;
			case XK_u:	return RH::UI::Events::KEY_U;
			case XK_v:	return RH::UI::Events::KEY_V;
			case XK_w:	return RH::UI::Events::KEY_W;
			case XK_x:	return RH::UI::Events::KEY_X;
			case XK_y:	return RH::UI::Events::KEY_Y;
			case XK_z:	return RH::UI::Events::KEY_Z;
		}
		return RH::UI::Events::KEY_UNKNOWN;
	}

	template<typename _T>
	void PublishEvent(XKeyEvent * xkeyevent) {

		eventPublicationManager.PublishEvent( _T(ToKey(xkeyevent)) );
	}

public:

	Input_(Display * dpy, Window window, long xEventsMask)
	{
		this->dpy = dpy;
		this->window = window;
		this->xEventsMask = xEventsMask;
	}

	void DispatchInputEvents() {

		XEvent xevent;
		XEvent *event = &xevent;

		if(XCheckWindowEvent(dpy, window, xEventsMask, event) == True) {

			switch( event->type ) {
			default:
				return;
			case KeyPress:
				PublishEvent<RH::UI::Events::KeyPressedEvent>(&xevent.xkey);
				return;
			case KeyRelease:
				PublishEvent<RH::UI::Events::KeyReleasedEvent>(&xevent.xkey);
				return;
			case ButtonPress:
				return;
			case ButtonRelease:
				return;
			case MotionNotify:
				return;
			}
		}
	}
};

