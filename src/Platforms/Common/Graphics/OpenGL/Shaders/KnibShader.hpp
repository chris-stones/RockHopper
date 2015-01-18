
#pragma once

class KnibShader_
	:	public ShaderBase_
{
	GLint mvpUniformLocation;
	GLint textureUniformLocationY;
	GLint textureUniformLocationCb;
	GLint textureUniformLocationCr;
	GLint textureUniformLocationA;
	GLuint vertexPositionAttributeLocation;
	GLuint texturePositionAttributeLocation;

	const bool alpha;

public:
	KnibShader_(GLuint program, bool alpha)
		:	ShaderBase_(program),
		 	alpha(alpha)
	{
		glUseProgram(program);
		mvpUniformLocation = this->GetUniformLocation("MVP");
		textureUniformLocationY  = this->GetUniformLocation("texSamp0");
		textureUniformLocationCb = this->GetUniformLocation("texSamp1");
		textureUniformLocationCr = this->GetUniformLocation("texSamp2");
		if(alpha)
			textureUniformLocationA  = this->GetUniformLocation("texSamp3");
		else
			textureUniformLocationA  = 0;
		vertexPositionAttributeLocation = this->GetAttributeLocation("positionAttr");
		texturePositionAttributeLocation = this->GetAttributeLocation("texcoordAttr");
	}

	void SetTextureSampler(int samplerY,int samplerCb,int samplerCr) {
		glUniform1i( textureUniformLocationY, samplerY);
		glUniform1i( textureUniformLocationCb, samplerCb);
		glUniform1i( textureUniformLocationCr, samplerCr);
	}

	void SetTextureSampler(int samplerY,int samplerCb,int samplerCr, int samplerA) {

		SetTextureSampler(samplerY, samplerCb, samplerCr);
		if(alpha)
			glUniform1i( textureUniformLocationA,  samplerA );
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

