#pragma once

#include <stdio.h>
#include <stdlib.h>

static int __report_gl_err(const char * file, const char * func, int line) {

	GLenum e;
	int errors = 0;

	while( (e = glGetError()) != GL_NO_ERROR ) {

		printf("RH %s:%s:%d gl error 0x%x (%d)\n", file, func, line, e, e);
		errors++;
		getchar();
	}

	return errors;
}
#define GL_ERROR() __report_gl_err(__FILE__,__FUNCTION__,__LINE__)

#include "Shaders/Shaders.hpp"
#include "TexPak/TexPak.hpp"
#include "UsesTextureFactory.hpp"
#include "TextureFactory.hpp"
#include "Sprite/Sprite.hpp"
#include "Mesh/Mesh.hpp"
#include "Resource/Bitmap.hpp"
#include "Resource/MotionVideo.hpp"
