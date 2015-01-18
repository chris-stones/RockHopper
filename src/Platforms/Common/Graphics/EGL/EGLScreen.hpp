#pragma once

class EGLScreen_ {

	EGLint visID;
	EGLConfig config;

	static const EGLint * _get_egl_attribs() {

	  static const EGLint attribs[] = {
		  EGL_RENDERABLE_TYPE, 	EGL_OPENGL_ES2_BIT,
		  EGL_SURFACE_TYPE, 	EGL_WINDOW_BIT,
		  EGL_BLUE_SIZE, 	8,
		  EGL_GREEN_SIZE, 	8,
		  EGL_RED_SIZE, 	8,
		  EGL_ALPHA_SIZE,	8,
		  EGL_NONE
	  };
	  return attribs;
	}

public:

	EGLScreen_(EGLDisplay dpy) {

		EGLint numConfigs;

		eglChooseConfig( dpy, _get_egl_attribs(), &config, 1, &numConfigs );

//		if(numConfigs > 0) {
//		  EGLint bufSize,r,g,b,a,z,s = 0;
//		  eglGetConfigAttrib(dpy,&config,EGL_BUFFER_SIZE,&bufSize);
//		  eglGetConfigAttrib(dpy,config,EGL_RED_SIZE,&r);
//		  eglGetConfigAttrib(dpy,config,EGL_GREEN_SIZE,&g);
//		  eglGetConfigAttrib(dpy,config,EGL_BLUE_SIZE,&b);
//		  eglGetConfigAttrib(dpy,config,EGL_ALPHA_SIZE,&a);
//		  eglGetConfigAttrib(dpy,config,EGL_DEPTH_SIZE,&z);
//		  eglGetConfigAttrib(dpy,config,EGL_STENCIL_SIZE,&s);

//		  printf("%d Configs found:\n\tFrame buffer(%d) RGBA(%d %d %d %d)\n\tZBuffer(%d) Z(%d) S(%d)\n\n",
//			numConfigs,bufSize,r,g,b,a,z+s,z,s);
//		}
		eglGetConfigAttrib(dpy, config, EGL_NATIVE_VISUAL_ID, &visID);
	}

	~EGLScreen_() {

	}

	EGLConfig GetConfig() {

		return config;
	}

	EGLint GetVisualID() {

		return visID;
	}
};
