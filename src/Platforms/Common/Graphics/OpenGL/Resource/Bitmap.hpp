#pragma once

#include <Graphics/Graphics.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class Bitmap::Impl
	: public UsesTextureFactory_
{
	RH::Libs::TexPak::Texture * texture {};
public:
	Impl(const std::string & s) {

		texture = GetTextureFactory()->GetTexture( s.c_str() );
	}

	virtual ~Impl() {

		delete texture;
	}

	GLuint GetTexture() {

		return texture->GetTexture();
	}

	void ReadCoords(int dim, int stride, float * coords) const {

		texture->ReadCoords(dim, stride, coords);
	}

	float GetWidth() const {

		return texture->GetWidth();
	}

	float GetHeight() const {

		return texture->GetHeight();
	}
};

Bitmap::Bitmap(const std::string & s) {

	impl = new Impl(s);
}

Bitmap::~Bitmap() {

	delete impl;
}

// IResource
float Bitmap::GetWidth() const {

	return impl->GetWidth();
}

// IResource
float Bitmap::GetHeight() const {

	return impl->GetHeight();
}

// IResource
void Bitmap::ReadCoords(int dim, int stride, float * coords) const {

	return impl->ReadCoords(dim, stride, coords);
}

}}}

