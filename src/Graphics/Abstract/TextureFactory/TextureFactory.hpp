
#pragma once

namespace RH { namespace Graphics {

class TextureFactory {
public:
	TextureFactory();
	~TextureFactory();

	class Impl;
	Impl * GetImpl() { return this->impl; }

private:

	Impl * impl;
};

}}

