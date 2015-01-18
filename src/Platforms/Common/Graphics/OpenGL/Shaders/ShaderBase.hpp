
#pragma once

class ShaderBase_ {

	GLuint program;

public:

	ShaderBase_(GLuint program)
		:	program(program)
	{
	}

	virtual ~ShaderBase_() {

		glDeleteProgram(program);
	}

	void Use() {

		glUseProgram(program);
	}

	GLint GetUniformLocation(const char * uniform) {

		return glGetUniformLocation(program, uniform);
	}

	GLuint GetAttributeLocation(const char * attribute) {

		return glGetAttribLocation(program, attribute);
	}

	void VertexAttribPointer(GLuint loc, GLint size, GLenum type, GLboolean normalise, GLsizei stride, const void * pointer) {

		glVertexAttribPointer(
			loc,size, type, normalise, stride,pointer);
	}

	void EnableVertexAttribArray(GLuint loc) {

		glEnableVertexAttribArray(loc);
	}

	void DisableVertexAttribArray(GLuint loc) {

		glDisableVertexAttribArray(loc);
	}
};

