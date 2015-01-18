
#pragma once

#include <Graphics/Graphics.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class Sprite::Impl
{
	GLuint vbuffer {0};
	std::shared_ptr<IResource> resource;

public:

	Impl(std::shared_ptr<IResource> resource)
		:	resource(resource)
	{
		glGenBuffers(1, &vbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		GLfloat vertex[16];
		resource->ReadCoords(2,4,vertex + 2);
		float w = resource->GetWidth() * 0.5f;
		float h = resource->GetHeight() * 0.5f;

		vertex[ 0] = vertex[ 4] = -w;
		vertex[ 8] = vertex[12] =  w;
		vertex[ 1] = vertex[ 9] = -h;
		vertex[ 5] = vertex[13] =  h;

		glBufferData(GL_ARRAY_BUFFER, sizeof vertex, vertex, GL_STATIC_DRAW);
	}
	~Impl() {
		glDeleteBuffers(1, &vbuffer);
	}

	GLuint GetVertexBuffer() const {
		return vbuffer;
	}

	std::shared_ptr<IResource> GetResource() {

		return resource;
	}

	glm::vec3 AlignmentCorrection() const {

		float w = resource->GetWidth() * (0.5f);
		float h = resource->GetHeight() * (0.5f);

		return glm::vec3(w,h,0.0f);
	}

	GLint  VertexPosSize() const { return 2; }
	GLenum VertexPosType() const { return GL_FLOAT; }
	GLboolean VertexPosNormalise() const { return GL_FALSE; }
	GLsizei VertexPosStride() const { return 4 * sizeof(GLfloat); }
	const void * VertexPosPointer() const { return (const void *)(0 * sizeof(GLfloat)); }
	GLint  TexturePosSize() const { return 2; }
	GLenum TexturePosType() const { return GL_FLOAT; }
	GLboolean TexturePosNormalise() const { return GL_FALSE; }
	GLsizei TexturePosStride() const { return 4 * sizeof(GLfloat); }
	const void * TexturePosPointer() const { return (const void *)(2 * sizeof(GLfloat)); }
};

Sprite::Sprite(std::shared_ptr<IResource> resource) {

	impl = new Impl(resource);
}

Sprite::~Sprite() {

	delete impl;
}

glm::vec3 Sprite::AlignmentCorrection() const {

	return impl->AlignmentCorrection();
}

}}}
