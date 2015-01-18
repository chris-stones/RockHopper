
#pragma once

class X11Dpy_ {
	Display* dpy;
public:
	X11Dpy_() {
		dpy = XOpenDisplay(NULL);
	}
	~X11Dpy_() {
		XCloseDisplay(dpy);
	}
	Display * Get() { return dpy; }
};
