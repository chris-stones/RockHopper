
#pragma once

#if(HAVE_GLX)

class GLXScreen_ {

	std::shared_ptr<X11Dpy_>	display;
	GLXFBConfig				*glxFBConfig;
	int						glxFBConfigSize;
	XVisualInfo				*xVisualInfo;
	XRRScreenConfiguration	*screenConfig;
	XRRScreenResources	 	*res;

	static const int * _get_glx_attribs() {

		static const int attribs[] = {

			GLX_RENDER_TYPE,		GLX_RGBA_BIT,
			GLX_X_RENDERABLE,		True,
			GLX_DRAWABLE_TYPE,		GLX_WINDOW_BIT,
			GLX_DOUBLEBUFFER,		True,
			GLX_RED_SIZE,			8,
			GLX_GREEN_SIZE,			8,
			GLX_BLUE_SIZE,			8,
			0
		};
		return attribs;
	}

public:
	GLXScreen_(std::shared_ptr<X11Dpy_> display, int screen_index) {

		this->display = display;

		Display * dpy =
			display->Get();

		glxFBConfig		= glXChooseFBConfig(dpy, screen_index, _get_glx_attribs(), &glxFBConfigSize);
		xVisualInfo		= glXGetVisualFromFBConfig(dpy, glxFBConfig[0] );
		screenConfig	= XRRGetScreenInfo( dpy, RootWindow(dpy, screen_index ) );
		res				= XRRGetScreenResources(dpy, RootWindow(dpy,screen_index));
	}

	~GLXScreen_() {

		XFree( glxFBConfig );
		XFree( xVisualInfo );
		XFree( res );
		XRRFreeScreenConfigInfo(screenConfig);
	}

	X11Dpy_ * GetDisplay() {

		return display.get();
	}

	XVisualInfo	* GetXVisualInfo() {

		return xVisualInfo;
	}

	GLXFBConfig * GetGLXFBConfig() {

		return glxFBConfig;
	}
};

#endif /*** HAVE_GLX ***/
