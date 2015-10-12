#pragma once

#include "IConcurrentJob.hpp"

namespace RH { namespace Libs { namespace Concurrency {

	class IConcurrentJobQueue {
	public:
		virtual ~IConcurrentJobQueue() {}
		virtual void AddJob(std::weak_ptr<IConcurrentJob> job) = 0;
	};
}}}
