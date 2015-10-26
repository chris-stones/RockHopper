#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(RockHopper, 0.1, chris.stones@zoho.com)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/Main.cpp)
AC_CONFIG_HEADERS(src/config.h)

AC_CONFIG_MACRO_DIR([m4])

#LT_INIT
AM_INIT_AUTOMAKE([subdir-objects])


AC_CANONICAL_HOST
case "$host" in
"*mingw*")
  platform="Win32"
  ;;
*)
  platform="GNULinux"
  ;;
esac

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

AC_ARG_WITH([gl],     AS_HELP_STRING([--with-gl],     [Use OpenGL.]))
AC_ARG_WITH([gles],   AS_HELP_STRING([--with-gles],   [Use OpenGLES.]))
AC_ARG_WITH([egl],    AS_HELP_STRING([--with-egl],    [Use EGL.]))
AC_ARG_WITH([glx],    AS_HELP_STRING([--with-glx],    [Use GLX.]))
AC_ARG_WITH([x11],    AS_HELP_STRING([--with-x11],    [Use X11.]))
AC_ARG_WITH([SDL2],   AS_HELP_STRING([--with-SDL2],   [Use SDL2.]))
AC_ARG_WITH([knib],   AS_HELP_STRING([--with-knib],   [Use Knib videos.]))
AC_ARG_WITH([theora], AS_HELP_STRING([--with-theora], [Use Theora videos.]))

#RASPBERY PI - Try to help the user as much as possible! 
AC_ARG_WITH(
  [raspberrypi],
  AS_HELP_STRING([--with-raspberrypi=SYSROOT_DIR], [Target RaspberryPi. Selects --with-gles]),
  [
    platform="RaspberryPi"
    with_egl="yes"
    with_gles="yes"
    with_gl="no"
    with_glx="no"
    with_x11="no"
    RPI_SYSROOT="$withval"
    CPPFLAGS="$CPPFLAGS -I$RPI_SYSROOT/opt/vc/include -I$RPI_SYSROOT/opt/vc/include/interface/vcos/pthreads -I$RPI_SYSROOT/opt/vc/include/interface/vmcs_host/linux"
    LDFLAGS="$LDFLAGS -L$RPI_SYSROOT/opt/vc/lib -Wl,--rpath-link,$RPI_SYSROOT/opt/vc/lib,--rpath,/opt/vc/lib"
  
    AC_SEARCH_LIBS(
    	[bcm_host_init],[bcm_host],[],
    	AC_MSG_ERROR([Unable to find bcm_host library.])
    )      
    AC_CHECK_HEADERS([bcm_host.h],[],[AC_MSG_ERROR([Missing bcm_host.h])])
  ]
 )

#FAKE RASPBERY PI -  skip the bcm_host headers and libraries, use X11 instead!
AC_ARG_WITH(
  [fake-raspberrypi],
  AS_HELP_STRING([--with-fake-raspberrypi], [Target Debug the PI on non-pi devices. Selects --with-gles]),
  [
    platform="RaspberryPi"
    with_egl="yes"
    with_gles="yes"
    with_gl="no"
    with_glx="no"
    with_x11="yes"
  ]
 )

AS_IF([test "x$with_gl" = "xyes"],
[
  AC_SEARCH_LIBS(
    [glClear],[GL],[AC_DEFINE([HAVE_OPENGL],[1],[use opengl])],
    AC_MSG_ERROR([Unable to find OpenGL library]))
  AC_CHECK_HEADERS([GL/gl.h],[],[AC_MSG_ERROR([Missing GL/gl.h])])
  AC_CHECK_HEADERS(
    [GL/glext.h],[],
    [AC_MSG_ERROR([Missing GL/glext.h])],[#include<GL/gl.h>])
])

AS_IF([test "x$with_x11" = "xyes"],
[
  AC_SEARCH_LIBS([XOpenDisplay],[X11],[],AC_MSG_ERROR([Unable to find X11 library]))
  AC_CHECK_HEADERS([X11/Xlib.h],[],[AC_MSG_ERROR([Missing X11/Xlib.h])])
  
  AC_SEARCH_LIBS([XRRGetScreenInfo],[Xrandr],[],AC_MSG_ERROR([Unable to find Xrandr library]))
  AC_CHECK_HEADERS([X11/extensions/Xrandr.h],[],[AC_MSG_ERROR([X11/extensions/Xrandr.h])])
  
  AC_SEARCH_LIBS([XineramaQueryScreens],[Xinerama],[],AC_MSG_ERROR([Unable to find Xinerama library]))
  AC_CHECK_HEADERS([X11/extensions/Xinerama.h],[],[AC_MSG_ERROR([Missing X11/extensions/Xinerama.h])])
])

AS_IF([test "x$with_glx" = "xyes"],
[
  AC_CHECK_HEADERS([GL/glx.h],
    [
      AC_DEFINE([HAVE_GLX],[1],[use glx])
    ],[AC_MSG_ERROR([Missing GL/glx.h])])
  AC_CHECK_HEADERS([GL/glxext.h],[],[AC_MSG_ERROR([Missing GL/glxext.h])],[#include<GL/glx.h>])
])

AS_IF([test "x$with_gles" = "xyes"],
[
  AC_SEARCH_LIBS(
    [glClear],[GLESv2],
    [
      AC_DEFINE([HAVE_GLES2],[1],[use gles2])
      AC_DEFINE([FRAG_SHADER_PRECISION],["precision mediump float;\n"],[default fragment shader precision])
    ],
    AC_MSG_ERROR([Unable to find OpenGLES library (libGLESv2.so)]))
    
  AC_CHECK_HEADERS([GLES2/gl2.h],[],[AC_MSG_ERROR([Missing GLES2/gl2.h])])
  AC_CHECK_HEADERS(
    [GLES2/gl2ext.h],[],
    [AC_MSG_ERROR([Missing GLES2/gl2ext.h])],[#include<GLES2/gl2.h>])
])

AS_IF([test "x$with_egl" = "xyes"],
[
  AC_SEARCH_LIBS(
    [eglTerminate],[EGL],
    [
      AC_DEFINE([HAVE_EGL],[1],[use EGL])
    ],
    AC_MSG_ERROR([Unable to find EGL library (libEGL.so)]))
 
  AC_CHECK_HEADERS([EGL/egl.h],[],[AC_MSG_ERROR([Missing EGL/egl.h])])
])

AS_IF([test "x$with_SDL2" = "xyes"],
[
  AC_SEARCH_LIBS(
    [SDL_Init],[SDL2],
    [
      AC_DEFINE([HAVE_SDL2],[1],[use SDL2])
    ],
    AC_MSG_ERROR([Unable to find SDL2 library (libSDL2.so)]))
  
  AC_CHECK_HEADERS([SDL2/SDL.h],[],[AC_MSG_ERROR([Missing SDL2/SDL.h])])
])

AC_SEARCH_LIBS([rh_texpak_open],[rh_texture_loader],[],AC_MSG_ERROR([Unable to find rh_texture_loader library]))
AC_CHECK_HEADERS(
	[rh_texture_loader.h],
	[],
	[AC_MSG_ERROR([Missing rh_texture_loader.h])],
	[
		#if HAVE_GLES2_GL2_H
  		#include <GLES2/gl2.h>
  		#endif
  		#if HAVE_GL_GL_H
  		#include <GL/gl.h>
  		#endif
	]
)

AS_IF([test "x$with_knib" = "xyes"],
[
	AC_DEFINE([HAVE_KNIB_VIDEO],[1],[use libknib_read for motion video support.])
	AC_SEARCH_LIBS([knib_open_file],[knib_read],[],AC_MSG_ERROR([Unable to find knib_reader library]))
	AC_CHECK_HEADERS(
		[knib_read.h],
		[],
		[AC_MSG_ERROR([Missing knib_read.h])],
		[
			#include<stdlib.h>
			#include<stddef.h>
		])
])

AS_IF([test "x$with_theora" = "xyes"],
[
	AC_CHECK_HEADERS([ogg/ogg.h],[],[AC_MSG_ERROR([Missing ogg/ogg.h])])
	AC_CHECK_HEADERS([theora/theoradec.h],[],[AC_MSG_ERROR([Missing theora/theoradec.h])])
	
	AC_SEARCH_LIBS([ogg_sync_init],[ogg],[],AC_MSG_ERROR([Unable to find libogg library]))
	AC_SEARCH_LIBS([th_decode_headerin],[theoradec],[],AC_MSG_ERROR([Unable to find libogg library]))
	
	AC_DEFINE([HAVE_THEORA_VIDEO],[1],[use libtheora and libogg for motion video support.])
])

AX_PTHREAD(
  [
    LIBS="$PTHREAD_LIBS $LIBS"
    CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
    CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
  ],
  [AC_MSG_ERROR([No pthreads!?])])

# TODO: Check fails due to c++11...
#AC_LANG_PUSH([C++])
#AC_CHECK_HEADERS(
#	[EventDispatcher/EventDispatcher.hpp],
#	[],
#	[AC_MSG_ERROR([Missing EventDispatcher/EventDispatcher.hpp])],
#	[
#	]
#)
#AC_LANG_POP([C++])

AM_CONDITIONAL([platform_GNULinux], [test "$platform" = "GNULinux"])
AM_CONDITIONAL([platform_RaspberryPi], [test "$platform" = "RaspberryPi"])
AM_CONDITIONAL([platform_Win32], [test "$platform" = "Win32"])

AC_PROG_CXX
AC_PROG_INSTALL

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT


