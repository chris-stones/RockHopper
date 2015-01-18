
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Mesh : public Object {

public:

	struct FormatXYZUV {
		float x;
		float y;
		float z;
		float u;
		float v;
	};

	Mesh(const std::vector<FormatXYZUV> &mesh);

	virtual ~Mesh();

	class Impl;

	Impl * GetImpl() { return impl; }

private:

	Impl * impl;
};

}}}

