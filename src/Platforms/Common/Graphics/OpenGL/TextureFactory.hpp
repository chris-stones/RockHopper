
#pragma once

#include <memory>

class TextureFactory_ {

	typedef std::unique_ptr<RH::Libs::TexPak::TexturePackage> UniqueTexPak;

	UniqueTexPak defautTexturePackage;

public:

	TextureFactory_() {

		int flags = RH::Libs::TexPak::TexturePackage::FILESYSTEM;

		defautTexturePackage = UniqueTexPak(
			new RH::Libs::TexPak::TexturePackage("default.texpak", flags));
	}

	~TextureFactory_() {

	}

	RH::Libs::TexPak::Texture * GetTexture(const char * name) {

		return defautTexturePackage->GetTexture(name);
	}
};

