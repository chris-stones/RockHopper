
#pragma once

class BasicShader_
	:	public ShaderBase_
{
	GLint mvpUniformLocation;
	GLint textureUniformLocation;
	GLuint vertexPositionAttributeLocation;
	GLuint texturePositionAttributeLocation;

public:
	BasicShader_(GLuint program)
		:	ShaderBase_(program)
	{
		glUseProgram(program);
		mvpUniformLocation = this->GetUniformLocation("MVP");
		textureUniformLocation = this->GetUniformLocation("texSamp");
		vertexPositionAttributeLocation = this->GetAttributeLocation("positionAttr");
		texturePositionAttributeLocation = this->GetAttributeLocation("texcoordAttr");
	}

	virtual ~BasicShader_() {

	}

	void SetTextureSampler(int sampler) {
		glUniform1i( textureUniformLocation, sampler);
	}

	void SetMVP(const glm::mat4 & MVP) {
		glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, glm::value_ptr(MVP) );
	}

	GLuint GetVertexPositionAttributeLocation() {
		return vertexPositionAttributeLocation;
	}

	GLuint GetTexturePositionAttributeLocation() {
		return texturePositionAttributeLocation;
	}


};

