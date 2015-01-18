
#pragma once

#ifdef HAVE_X11_XLIB_H
	#include <X11/Xlib.h>
	#include <X11/extensions/Xrandr.h>
	#include <X11/extensions/Xinerama.h>
#endif

#ifdef HAVE_GL_GL_H
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#ifdef HAVE_GL_GLEXT_H
		#include <GL/glext.h>
	#endif
#endif

#ifdef HAVE_GL_GLX_H
	#include <GL/glx.h>
#endif

#ifdef HAVE_GL_GLXEXT_H
	#include <GL/glxext.h>
#endif

#ifdef HAVE_GLES2_GL2_H
	#include <GLES2/gl2.h>
#endif

#ifdef HAVE_GLES2_GL2EXT_H
	#include <GLES2/gl2ext.h>
#endif

#ifdef HAVE_EGL_EGL_H
	#include <EGL/egl.h>
#endif

#ifndef ETC1_RGB8_OES
#define ETC1_RGB8_OES 0x8D64
#endif
#ifndef COMPRESSED_RGB_S3TC_DXT1_EXT
	#define COMPRESSED_RGB_S3TC_DXT1_EXT  0x83F0
#endif
#ifndef COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef COMPRESSED_RGBA_S3TC_DXT3_EXT
	#define COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef COMPRESSED_RGBA_S3TC_DXT5_EXT
	#define COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
