#pragma once

class EGL_ {

	EGLDisplay dpy;
	EGLContext egl_ctx;

public:

	EGL_(EGLDisplay dpy, EGLConfig config) {

		this->dpy = dpy;

		const EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

		egl_ctx = eglCreateContext(dpy, config, EGL_NO_CONTEXT, attrib_list);
	}

	~EGL_() {

		eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(dpy, egl_ctx);
	}

	EGLContext GetContext() const {return egl_ctx; }
};

