
#include <Windows.h>

//#define GL_GLEXT_PROTOTYPES 1

#include <GL/glew.h>

#include "opengl_org\GL\glcorearb.h"
#include "opengl_org\GL\glext.h"
#include "opengl_org\GL\wglext.h"
#define HAVE_OPENGL 1

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
