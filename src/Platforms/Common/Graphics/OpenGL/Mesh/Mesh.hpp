
#pragma once

#include <Graphics/Graphics.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class Mesh::Impl {

	GLuint vbuffer {0};

public:

	Impl(const std::vector<FormatXYZUV> &mesh) {
		glGenBuffers(1, &vbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FormatXYZUV) * mesh.size(), &mesh[0], GL_STATIC_DRAW);
	}

	~Impl() {
		glDeleteBuffers(1, &vbuffer);
	}

	GLuint GetVertexBuffer() const {

		return vbuffer;
	}

	GLint  VertexPosSize() const { return 3; }
	GLenum VertexPosType() const { return GL_FLOAT; }
	GLboolean VertexPosNormalise() const { return GL_FALSE; }
	GLsizei VertexPosStride() const { return 5 * sizeof(GLfloat); }
	const void * VertexPosPointer() const { return (const void *)(0 * sizeof(GLfloat)); }
	GLint  TexturePosSize() const { return 2; }
	GLenum TexturePosType() const { return GL_FLOAT; }
	GLboolean TexturePosNormalise() const { return GL_FALSE; }
	GLsizei TexturePosStride() const { return 5 * sizeof(GLfloat); }
	const void * TexturePosPointer() const { return (const void *)(3 * sizeof(GLfloat)); }
};

Mesh::Mesh(const std::vector<FormatXYZUV> &mesh) {

	impl = new Impl(mesh);
}

Mesh::~Mesh() {

	delete impl;
}

}}}

