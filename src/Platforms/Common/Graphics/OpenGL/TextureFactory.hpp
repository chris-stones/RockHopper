
#pragma once

#include<Graphics/Abstract/TextureFactory/TextureFactory.hpp>

namespace RH { namespace Graphics {

class TextureFactory::Impl {

	typedef std::unique_ptr<RH::Libs::TexPak::TexturePackage> UniqueTexPak;

	UniqueTexPak defautTexturePackage;

public:

	Impl() {

		int flags = RH::Libs::TexPak::TexturePackage::FILESYSTEM;

		defautTexturePackage = UniqueTexPak(
			new RH::Libs::TexPak::TexturePackage("default.texpak", flags));
	}

	virtual ~Impl() {

	}

	RH::Libs::TexPak::Texture * GetTexture(const char * name) {

		return defautTexturePackage->GetTexture(name);
	}
};

TextureFactory::TextureFactory() {

	impl = new Impl();
}

TextureFactory::~TextureFactory() {

	delete impl;
}


}}
