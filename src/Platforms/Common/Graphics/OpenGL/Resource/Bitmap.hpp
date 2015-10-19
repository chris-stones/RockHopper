#pragma once

#include <Graphics/Graphics.hpp>
#include <Libs/IoCC/IoCC.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class Bitmap::Impl {

	RH::Libs::TexPak::Texture * texture {};
public:
	Impl(const char * resource, std::shared_ptr<TextureFactory> tf) {

		texture = tf->GetImpl()->GetTexture(resource);
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

Bitmap::Bitmap(const char * resource, std::shared_ptr<TextureFactory> tf) {

	impl = new Impl(resource, tf);
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

