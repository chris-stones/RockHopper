#pragma once

namespace RH { namespace Libs { namespace Concurrency {

	class IConcurrentUpdatable {
	public:
		virtual ~IConcurrentUpdatable() {}
		virtual void ConcurrentUpdate() = 0;
	};
}}}
