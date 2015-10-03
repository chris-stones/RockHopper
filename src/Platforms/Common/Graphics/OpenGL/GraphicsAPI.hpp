#pragma once

namespace RH { namespace Graphics {

class GraphicsAPI::Impl
	:	public Abstract::OperationVisitor
{
	UI::Window::Impl * windowImpl {};
	Abstract::Sprite::Impl * spriteImpl {};

	std::unique_ptr<BasicShader_> basicShader;
	std::unique_ptr<KnibShader_>  knibShaders[3];
	std::unique_ptr<KnibShader_>  knibShadersAlpha[3];

public:

	Impl() {

		ShaderFactory_ sf;

		GL_ERROR();

		basicShader = std::unique_ptr<BasicShader_>(sf.CreateBasicShader());
		for(int i=0;i<3;i++)
			knibShaders[i] = std::unique_ptr<KnibShader_>(sf.CreateKnibShader(i, false));
		for(int i=0;i<3;i++)
			knibShadersAlpha[i] = std::unique_ptr<KnibShader_>(sf.CreateKnibShader(i, true));

		GL_ERROR();
	}

	~Impl() {

	}

	void Begin(UI::Window * window) {

		this->windowImpl = window->GetImpl();

		GL_ERROR();
		windowImpl->MakeContextCurrent();
		GL_ERROR();
//		glClearColor(1.0f,0.0f,1.0f,1.0f);
//		GL_ERROR();
//		glClear( GL_COLOR_BUFFER_BIT );
		GL_ERROR();
	}

	void Render(Abstract::Object * object, const glm::mat4 & MVP, float alpha) {

		basicShader->SetMVP(MVP);
		for(int i=0;i<3;i++)
			knibShaders[i]->SetMVP(MVP);
		for(int i=0;i<3;i++)
			knibShadersAlpha[i]->SetMVP(MVP);

		object->AcceptVisitor(this);
	}

	void End() {

		windowImpl->SwapBuffers();
		GL_ERROR();
	}

	virtual void Visit( Abstract::Bitmap * bitmap ) {

		Abstract::Bitmap::Impl * bitmapImpl =
			bitmap->GetImpl();

		RenderRGBaTexture( bitmapImpl->GetTexture() );
	}

	void RenderRGBaTexture(GLuint RGBaTexture) {

		BasicShader_ * shader = basicShader.get();

		// Setup shader program
		GL_ERROR();
		shader->Use();
		GL_ERROR();
		glBindBuffer(GL_ARRAY_BUFFER,spriteImpl->GetVertexBuffer());
		GL_ERROR();
		shader->VertexAttribPointer(
			shader->GetVertexPositionAttributeLocation(),
			spriteImpl->VertexPosSize(),
			spriteImpl->VertexPosType(),
			spriteImpl->VertexPosNormalise(),
			spriteImpl->VertexPosStride(),
			spriteImpl->VertexPosPointer());
		GL_ERROR();
		shader->VertexAttribPointer(
			shader->GetTexturePositionAttributeLocation(),
			spriteImpl->TexturePosSize(),
			spriteImpl->TexturePosType(),
			spriteImpl->TexturePosNormalise(),
			spriteImpl->TexturePosStride(),
			spriteImpl->TexturePosPointer());
		GL_ERROR();
		shader->EnableVertexAttribArray(
			shader->GetVertexPositionAttributeLocation());
		GL_ERROR();
		shader->EnableVertexAttribArray(
			shader->GetTexturePositionAttributeLocation());
		GL_ERROR();
		shader->SetTextureSampler(0);
		GL_ERROR();
		glActiveTexture(GL_TEXTURE0);
		GL_ERROR();
		glBindTexture(GL_TEXTURE_2D, RGBaTexture);
		GL_ERROR();
	}

	void RenderPlanarKnib(Abstract::MotionVideo::Impl * motionVideoImpl) {

		KnibShader_ * shader;

		if(motionVideoImpl->HasAlphaChannel())
			shader = knibShadersAlpha[motionVideoImpl->ShaderIndex()].get();
		else
			shader = knibShaders[motionVideoImpl->ShaderIndex()].get();

		// Setup shader program
		GL_ERROR();
		shader->Use();
		GL_ERROR();
		glBindBuffer(GL_ARRAY_BUFFER,spriteImpl->GetVertexBuffer());
		GL_ERROR();
		shader->VertexAttribPointer(
			shader->GetVertexPositionAttributeLocation(),
			spriteImpl->VertexPosSize(),
			spriteImpl->VertexPosType(),
			spriteImpl->VertexPosNormalise(),
			spriteImpl->VertexPosStride(),
			spriteImpl->VertexPosPointer());
		GL_ERROR();
		shader->VertexAttribPointer(
			shader->GetTexturePositionAttributeLocation(),
			spriteImpl->TexturePosSize(),
			spriteImpl->TexturePosType(),
			spriteImpl->TexturePosNormalise(),
			spriteImpl->TexturePosStride(),
			spriteImpl->TexturePosPointer());
		GL_ERROR();
		shader->EnableVertexAttribArray(
			shader->GetVertexPositionAttributeLocation());
		GL_ERROR();
		shader->EnableVertexAttribArray(
			shader->GetTexturePositionAttributeLocation());
		GL_ERROR();
		shader->SetTextureSampler(0,1,2,3);
		GL_ERROR();
		glActiveTexture(GL_TEXTURE0+0);
		glBindTexture(GL_TEXTURE_2D, motionVideoImpl->GetYTexture());
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, motionVideoImpl->GetCbTexture());
		glActiveTexture(GL_TEXTURE0+2);
		glBindTexture(GL_TEXTURE_2D, motionVideoImpl->GetCrTexture());
		glActiveTexture(GL_TEXTURE0+3);
		glBindTexture(GL_TEXTURE_2D, motionVideoImpl->GetATexture());
		GL_ERROR();
	}

	virtual void Visit( Abstract::MotionVideo * motionVideo ) {

		Abstract::MotionVideo::Impl * motionVideoImpl =
			motionVideo->GetImpl();

		if(motionVideoImpl->GetYTexture())
			RenderPlanarKnib( motionVideoImpl );
		else if(motionVideoImpl->GetATexture()) {
			// TODO: KnibPacked With Alpha.
		}
		else
			RenderRGBaTexture( motionVideoImpl->GetRGBTexture() );
	}

	virtual void Visit( Abstract::Sprite * sprite ) {

		spriteImpl = sprite->GetImpl();
		spriteImpl->GetResource()->AcceptVisitor(this);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		GL_ERROR();
	}

	virtual void Visit( Abstract::Mesh * mesh) {

//		Abstract::Mesh::Impl * meshImpl =
//			mesh->GetImpl();

		GL_ERROR();
	}
};

GraphicsAPI::GraphicsAPI() {
	impl = new Impl();
}
GraphicsAPI::~GraphicsAPI() {
	delete impl;
}
void GraphicsAPI::Begin(UI::Window * window) {
	impl->Begin(window);
}
void GraphicsAPI::Render(Abstract::Object * object, const glm::mat4 & MVP, float alpha) {
	impl->Render(object, MVP, alpha);
}
void GraphicsAPI::End() {
	impl->End();
}

}}
