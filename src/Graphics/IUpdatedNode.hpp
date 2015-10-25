#pragma once

namespace RH { namespace Graphics {

	class IUpdatedNode {
	public:
		virtual ~IUpdatedNode() {}
		virtual void UpdateNode() = 0;
	};

}}
