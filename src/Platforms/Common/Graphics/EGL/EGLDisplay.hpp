
#pragma once

class EGLDisplay_ {

	EGLDisplay dpy;

public:

	EGLDisplay_(EGLint api, EGLNativeDisplayType nativeDisplay) {

		EGLint vMajor, vMinor;

		if(!eglBindAPI(api))
			throw std::runtime_error("eglBindApi");

		if((dpy = eglGetDisplay(nativeDisplay)) == EGL_NO_DISPLAY)
			throw std::runtime_error("eglGetDisplay");

		if(!eglInitialize(dpy, &vMajor, &vMinor))
			throw std::runtime_error("eglInitialize");
	}

	~EGLDisplay_() {

		eglTerminate(dpy);
	}

	EGLDisplay Get() {

		return dpy;
	}
};

