
#pragma once

class EGLWindow_ {

	EGLDisplay dpy;
	EGLSurface surface;

	int w;
	int h;

public:

	EGLWindow_(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType nativewindow) {

		this->dpy = dpy;

		surface = eglCreateWindowSurface(
		  dpy,
		  config,
		  nativewindow,
		  NULL);

		eglQuerySurface(dpy, surface, EGL_WIDTH,  &w);
		eglQuerySurface(dpy, surface, EGL_HEIGHT, &h);
	}

	~EGLWindow_() {

		eglDestroySurface(dpy, surface);
	}

	EGLSurface GetSurface() const { return surface; }
};
