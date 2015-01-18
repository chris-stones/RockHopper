
#pragma once

#if(HAVE_GLX)

class GLX_
{
	std::shared_ptr<GLXScreen_> screen;
	GLXContext glx_ctx;

	/* code stolen from http://www.opengl.org/archives/resources/features/OGLextensions/ */
	static bool IsExtensionSupported(const char *extension)
	{
	  const GLubyte *extensions = NULL;
	  const GLubyte *start;
	  GLubyte *where, *terminator;

	  /* Extension names should not have spaces. */
	  where = (GLubyte *) strchr(extension, ' ');
	  if (where || *extension == '\0')
	    return 0;
	  extensions = glGetString(GL_EXTENSIONS);
	  /* It takes a bit of care to be fool-proof about parsing the
	     OpenGL extensions string. Don't be fooled by sub-strings,
	     etc. */
	  start = extensions;
	  for (;;) {
	    where = (GLubyte *) strstr((const char *) start, extension);
	    if (!where)
	      break;
	    terminator = where + strlen(extension);
	    if (where == start || *(where - 1) == ' ')
	      if (*terminator == ' ' || *terminator == '\0')
	        return true;
	    start = terminator;
	  }
	  return false;
	}

public:

	GLX_( std::shared_ptr<GLXScreen_> screen, int major_version, int minor_version, GLX_ * shareCtx)
		:	screen(screen),
		 	glx_ctx(nullptr)
	{
		Display * dpy = screen->GetDisplay()->Get();

		GLXFBConfig framebufferConfig = screen->GetGLXFBConfig()[0]; // TODO: using first config for now.

		if(major_version > 2) {

			typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

			glXCreateContextAttribsARBProc glXCreateContextAttribsARB =
			  (glXCreateContextAttribsARBProc)
				(glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" ) );

			if( IsExtensionSupported("GLX_ARB_create_context") && glXCreateContextAttribsARB) {

			  const int context_attribs[] = {
				  GLX_CONTEXT_MAJOR_VERSION_ARB, major_version,
				  GLX_CONTEXT_MINOR_VERSION_ARB, minor_version,
				  //GLX_CONTEXT_FLAGS_ARB,
				  //GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				  None
			  };

			  glx_ctx = glXCreateContextAttribsARB( dpy, framebufferConfig, shareCtx ? shareCtx->glx_ctx : NULL, True, context_attribs);
			}
		}
		else
		  glx_ctx = glXCreateNewContext( dpy, framebufferConfig, GLX_RGBA_TYPE, shareCtx ? shareCtx->glx_ctx : NULL, True );
	}

	~GLX_() {

		glXMakeCurrent( screen->GetDisplay()->Get(), None, NULL);
		glXDestroyContext ( screen->GetDisplay()->Get(), glx_ctx );
	}

	GLXContext Get() {

		return glx_ctx;
	}
};

#endif /*** HAVE_GLX ***/
