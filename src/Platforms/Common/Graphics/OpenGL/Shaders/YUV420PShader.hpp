
#pragma once

class YUV420PShader_
	:	public ShaderBase_
{
	GLint mvpUniformLocation;
	GLint textureUniformLocationY;
	GLint textureUniformLocationCb;
	GLint textureUniformLocationCr;;
	GLuint vertexPositionAttributeLocation;
	GLuint texturePositionAttributeLocation;

public:
	YUV420PShader_(GLuint program)
		:	ShaderBase_(program)
	{
		glUseProgram(program);
		mvpUniformLocation = this->GetUniformLocation("MVP");
		textureUniformLocationY = this->GetUniformLocation("texSampY");
		textureUniformLocationCb = this->GetUniformLocation("texSampCb");
		textureUniformLocationCr = this->GetUniformLocation("texSampCr");
		vertexPositionAttributeLocation = this->GetAttributeLocation("positionAttr");
		texturePositionAttributeLocation = this->GetAttributeLocation("texcoordAttr");
	}

	void SetTextureSampler(int samplerY,int samplerCb,int samplerCr) {
		glUniform1i( textureUniformLocationY, samplerY);
		glUniform1i( textureUniformLocationCb, samplerCb);
		glUniform1i( textureUniformLocationCr, samplerCr);
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

