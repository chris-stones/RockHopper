#pragma once

typedef enum {
	_NET_WM_STATE_REMOVE = 0, _NET_WM_STATE_ADD = 1, _NET_WM_STATE_TOGGLE = 2

} _net_wm_state_enum_t;


class X11Window_ {

	Display * dpy;
//	XVisualInfo * vis;
	int vis_screen;

	bool fs;

	long xEventsMask;
	Window window;

	void SetBorderless() {

		/*** Stole this code from Stack Overflow
		 * MOTIF hints are pretty out of date, but there doesn't seem to be a _NET_* alternative ?
		 */

		struct MwmHints {
			unsigned long flags;
			unsigned long functions;
			unsigned long decorations;
			long input_mode;
			unsigned long status;
		};

		enum {
			MWM_HINTS_FUNCTIONS = (1L << 0),
			MWM_HINTS_DECORATIONS = (1L << 1),
			MWM_FUNC_ALL = (1L << 0),
			MWM_FUNC_RESIZE = (1L << 1),
			MWM_FUNC_MOVE = (1L << 2),
			MWM_FUNC_MINIMIZE = (1L << 3),
			MWM_FUNC_MAXIMIZE = (1L << 4),
			MWM_FUNC_CLOSE = (1L << 5)
		};

		Atom mwmHintsProperty = XInternAtom(dpy, "_MOTIF_WM_HINTS", 0);
		struct MwmHints hints;
		hints.flags = MWM_HINTS_DECORATIONS;
		hints.decorations = 0;
		XChangeProperty(dpy, window, mwmHintsProperty, mwmHintsProperty, 32,
				PropModeReplace, (unsigned char *) &hints, 5);
	}

	void SetPointerVisible(int vis) {

		if (vis) {

			XDefineCursor(dpy, window, None);
		} else {
			Pixmap bm_no;
			Colormap cmap;
			Cursor no_ptr;
			XColor black, dummy;
			static char bm_no_data[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

			cmap = DefaultColormap(dpy, DefaultScreen(dpy));
			XAllocNamedColor(dpy, cmap, "black", &black, &dummy);
			bm_no = XCreateBitmapFromData(dpy, window, bm_no_data, 8, 8);
			no_ptr = XCreatePixmapCursor(dpy, bm_no, bm_no, &black, &black, 0,
					0);

			XDefineCursor(dpy, window, no_ptr);
			XFreeCursor(dpy, no_ptr);
			if (bm_no != None)
				XFreePixmap(dpy, bm_no);
			XFreeColors(dpy, cmap, &black.pixel, 1, 0);
		}
	}

	void SetAboveOthers(int above) {

		Atom wmState = XInternAtom(dpy, "_NET_WM_STATE", False);
		Atom wmAbove = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", False);

		XEvent xev;
		xev.xclient.type = ClientMessage;
		xev.xclient.serial = 0;
		xev.xclient.send_event = True;
		xev.xclient.window = window;
		xev.xclient.message_type = wmState;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] =
				above ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
		xev.xclient.data.l[1] = wmAbove;
		xev.xclient.data.l[2] = 0;

		XSendEvent(dpy, RootWindow(dpy, vis_screen),
				False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
	}

	void SetFullscreenMonitors(const RH::UI::WindowHints & windowHints) {

		int x = windowHints.GetX();
		int y = windowHints.GetY();
		int w = windowHints.GetW();
		int h = windowHints.GetH();
		int i;

		int xineramaScreenInfoLength = 0;
		XineramaScreenInfo *xineramaScreenInfo = XineramaQueryScreens(dpy,
				&xineramaScreenInfoLength);

		int top = -1;
		int bot = -1;
		int left = -1;
		int right = -1;

		for (i = 0; i < xineramaScreenInfoLength; i++) {

			if (y == xineramaScreenInfo[i].y_org && top == -1)
				top = xineramaScreenInfo[i].screen_number;

			if (x == xineramaScreenInfo[i].x_org && left == -1)
				left = xineramaScreenInfo[i].screen_number;

			if (x + w - 1
					== xineramaScreenInfo[i].x_org + xineramaScreenInfo[i].width
							- 1 && right == -1)
				right = xineramaScreenInfo[i].screen_number;

			if (y + h - 1
					== xineramaScreenInfo[i].y_org
							+ xineramaScreenInfo[i].height - 1 && bot == -1)
				bot = xineramaScreenInfo[i].screen_number;

			if (y == xineramaScreenInfo[i].y_org
					&& x == xineramaScreenInfo[i].x_org
					&& x + w - 1
							== xineramaScreenInfo[i].x_org
									+ xineramaScreenInfo[i].width - 1
					&& y + h - 1
							== xineramaScreenInfo[i].y_org
									+ xineramaScreenInfo[i].height - 1) {

				top = right = bot = left = xineramaScreenInfo[i].screen_number;
			}
		}

		XFree(xineramaScreenInfo);

		if (top >= 0 && bot >= 0 && left >= 0 && right >= 0) {

			XEvent xev;
			memset(&xev, 0, sizeof xev);
			xev.xclient.type = ClientMessage;
			xev.xclient.serial = 0;
			//			xev.xclient.send_event=True;
			xev.xclient.window = window;
			xev.xclient.message_type = XInternAtom(dpy,
					"_NET_WM_FULLSCREEN_MONITORS", False);
			xev.xclient.format = 32;
			xev.xclient.data.l[0] = top;
			xev.xclient.data.l[1] = bot;
			xev.xclient.data.l[2] = left;
			xev.xclient.data.l[3] = right;
			xev.xclient.data.l[4] = 1; // source indication - 1 for normal application.

			XSendEvent(dpy, RootWindow(dpy, vis_screen), False,
					SubstructureRedirectMask | SubstructureNotifyMask, &xev);
		}
	}

	void SetFullscreenMode(const RH::UI::WindowHints & windowHints) {

		if ((this->fs) != windowHints.IsFullScreen()) {

			XEvent xev;
			xev.xclient.type = ClientMessage;
			xev.xclient.serial = 0;
			xev.xclient.send_event = True;
			xev.xclient.window = window;
			xev.xclient.message_type = XInternAtom(dpy, "_NET_WM_STATE", False);
			xev.xclient.format = 32;
			xev.xclient.data.l[0] =
					windowHints.IsFullScreen() ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
			xev.xclient.data.l[1] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN",
					False);
			xev.xclient.data.l[2] = 0;

			XSendEvent(dpy, RootWindow(dpy, vis_screen), False,
					SubstructureRedirectMask | SubstructureNotifyMask, &xev);

			this->fs = windowHints.IsFullScreen();
			if (this->fs)
				SetFullscreenMonitors(windowHints);
		}
	}

public:

	X11Window_(const RH::UI::WindowHints & windowHints, Display * dpy, XVisualInfo * vis)
		:	dpy(dpy),
		 	vis_screen(vis->screen)
	{

		XSetWindowAttributes attribs;
		memset(&attribs, 0, sizeof attribs);

		xEventsMask = NoEventMask;

		if (windowHints.DoesGenerateClickEvents())
			xEventsMask |= ButtonPressMask | ButtonReleaseMask;

		if (windowHints.DoesGenerateKeyEvents())
			xEventsMask |= KeyPressMask | KeyReleaseMask;

		if (windowHints.DoesGenerateMouseMoveEvents())
			xEventsMask |= PointerMotionMask;

		attribs.event_mask = xEventsMask;
		attribs.border_pixel = 0;
		attribs.bit_gravity = StaticGravity;

		if (windowHints.IsFullScreen())
			attribs.override_redirect = True;

		attribs.colormap = XCreateColormap(dpy, RootWindow(dpy, vis->screen),
				vis->visual, AllocNone);

		GLint winMask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;

		window = XCreateWindow(dpy, RootWindow(dpy, vis->screen), windowHints.GetX(),
				windowHints.GetY(),
				windowHints.GetW(), windowHints.GetH(), 0, vis->depth, InputOutput, vis->visual,
				winMask, &attribs);

		XMapWindow(dpy, window);

		XStoreName(dpy, window, "X11");

		if (windowHints.IsNotFullScreen()) {
			XMoveWindow(dpy, window, windowHints.GetX(), windowHints.GetY());
			if (windowHints.IsNotDecorated())
				SetBorderless();
		} else {

			SetFullscreenMode(windowHints);
			SetAboveOthers(true);
		}

		XFlush(dpy);
	}

	~X11Window_() {

		XDestroyWindow(dpy, window);
	}

	Display * GetX11Display() {

		return dpy;
	}

	Window GetX11Window() {

		return window;
	}

	long GetX11EventsMask() {

		return xEventsMask;
	}
};


