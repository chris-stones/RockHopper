#pragma once

#if(HAVE_GLX)


class GLXWindow_
	:	public X11Window_
{
	GLXWindow glxWindow;

public:

	GLXWindow_(const RH::UI::WindowHints & windowHints, Display * dpy, XVisualInfo * vis, GLXFBConfig glxFbConfig)
		:	X11Window_(windowHints, dpy, vis)
	{
		glxWindow = glXCreateWindow(dpy, glxFbConfig, GetX11Window(), 0);
	}

	~GLXWindow_() {

		glXDestroyWindow(GetX11Display(), glxWindow);
	}

	void SwapBuffers() {

		glXSwapBuffers(GetX11Display(), glxWindow);
	}

	GLXWindow GetGLXWindow() {

		return glxWindow;
	}
};

#endif /*** HAVE_GLX ***/

