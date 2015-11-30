#pragma once

#include "fbdev_window.h" // Mali400 Frame Buffer drivers header file. ( From Mali400 SDK )

class EGL_Mali400_Display_ {

	EGLDisplay dpy;
	EGLint version_major;
	EGLint version_minor;
	EGLNativeWindowType nativewindow;

	fbdev_window * mali400_window {nullptr};

public:

	EGL_Mali400_Display_() {

		mali400_window = (fbdev_window*)calloc(1, sizeof(fbdev_window));
		mali400_window->width = 1920;
		mali400_window->height = 1080;

		nativewindow = (EGLNativeWindowType)(mali400_window);

		if((dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY ) {

		  printf("eglGetDisplay failed!!!\n");
		  throw std::runtime_error("eglGetDisplay");
		}

		if( eglInitialize(dpy, &version_major, &version_minor) != EGL_TRUE ) {

		  printf("eglInitialize failed!!!\n");
		  throw std::runtime_error("eglInitialize");
		}
	}

	~EGL_Mali400_Display_() {

		free(mali400_window);
	}

	EGLDisplay Get() const {

		return this->dpy;
	}

	EGLNativeWindowType GetNativeWindow() {
		return nativewindow;
	}
};
