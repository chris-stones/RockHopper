
#pragma once

#include "TextureFactory.hpp"

class UsesTextureFactory_ {

	static TextureFactory_ *& TF() {

		static TextureFactory_ * textureFactory = NULL;
		return textureFactory;
	}

protected:

	static TextureFactory_ * GetTextureFactory() {

		return TF();
	}

public:

	static void SetTextureFactory(TextureFactory_ * tf) {

		TF() = tf;
	}
};

