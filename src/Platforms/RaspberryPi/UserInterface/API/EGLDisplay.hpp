#pragma once

class Display_ {

	EGLDisplay dpy;
	EGLint version_major;
	EGLint version_minor;
	EGLNativeWindowType nativewindow;
	EGL_DISPMANX_WINDOW_T nativewindow_obj;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	uint32_t display_width;
	uint32_t display_height;

public:

	Display_() {

		bcm_host_init();

		if( graphics_get_display_size(0 /* LCD */, &display_width, &display_height) < 0 ) {
		  bcm_host_deinit();
		  throw std::runtime_error("bcm::graphics_get_display_size");
		}

		dst_rect.x = 0;
		dst_rect.y = 0;
		dst_rect.width = display_width;
		dst_rect.height = display_height;

		// You can hardcode the resolution here:
		//display_width = 1024;
		//display_height = 768;

		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.width = display_width << 16;
		src_rect.height = display_height << 16;

		dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
		dispman_update = vc_dispmanx_update_start( 0 );

		dispman_element = vc_dispmanx_element_add ( dispman_update,
		  dispman_display, 0/*layer*/, &dst_rect, 0/*src*/,
		  &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/,
		  0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

		nativewindow_obj.element = dispman_element;
		nativewindow_obj.width = display_width;
		nativewindow_obj.height = display_height;

		nativewindow = (EGLNativeWindowType)(&nativewindow_obj);

		vc_dispmanx_update_submit_sync( dispman_update );

		if((dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY ) {

		  printf("eglGetDisplay failed!!!\n");
		  bcm_host_deinit();
		  throw std::runtime_error("eglGetDisplay");
		}

		if( eglInitialize(dpy, &version_major, &version_minor) != EGL_TRUE ) {

		  printf("eglInitialize failed!!!\n");
		  bcm_host_deinit();
		  throw std::runtime_error("eglInitialize");
		}
	}

	~Display_() {
		eglTerminate(dpy);
		bcm_host_deinit();
	}

	EGLDisplay Get() const {

		return this->dpy;
	}

	EGLNativeWindowType GetNativeWindow() {
		return nativewindow;
	}
};
