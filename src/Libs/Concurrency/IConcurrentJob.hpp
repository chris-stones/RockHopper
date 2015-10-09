#pragma once

namespace RH { namespace Libs { namespace Concurrency {

	class IConcurrentJob {
	public:
		virtual ~IConcurrentJob() {}
		virtual void ConcurrentJob() = 0;
	};
}}}
