#pragma once

#include <Libs/Concurrency/IConcurrentJob.hpp>
#include <Libs/Concurrency/IConcurrentJobQueue.hpp>
#include <thread>
#include <atomic>
#include <deque>
#include <vector>
#include <condition_variable>


namespace RH { namespace Libs { namespace Concurrency {

	class ConcurrentJobQueue : public IConcurrentJobQueue {

		typedef std::vector< std::thread> ThreadVector;
		typedef std::deque< std::weak_ptr<IConcurrentJob> > WorkDeque;

		ThreadVector 	threadVector;
		WorkDeque 		workDeque;
		const int		max_queue;
		bool		 	shutdown_flag {false};

		std::mutex mutex;
		std::condition_variable readable;

		std::shared_ptr<IConcurrentJob> GetWork() {

			std::unique_lock<std::mutex> lock( mutex );

			while( !shutdown_flag && ( workDeque.size() == 0) )
				readable.wait( lock );

			std::shared_ptr<IConcurrentJob> work;

			if(workDeque.size()) {

				work = workDeque.front().lock();

				workDeque.pop_front();
			}

			return work;
		}

	public:

		ConcurrentJobQueue(int threads)
			:	max_queue(threads)
		{
			threadVector.reserve(threads);
			for(int i=0;i<threads; i++)
				threadVector.push_back( std::thread( &ConcurrentJobQueue::Main, this ) );
		}

		virtual ~ConcurrentJobQueue() {

			{
				std::unique_lock<std::mutex> lock( mutex );
				shutdown_flag = true;
				readable.notify_all();
			}
			for(unsigned int i=0;i<threadVector.size(); i++)
				threadVector[i].join();
		}

		virtual void AddJob(std::weak_ptr<IConcurrentJob> work) override {

			std::unique_lock<std::mutex> lock( mutex );
			workDeque.push_back(work);
			readable.notify_all();
		}

	private:

		void Main() {

			std::shared_ptr<IConcurrentJob> work;

			while(!this->shutdown_flag) {

				if((work = GetWork())) {
					work->ConcurrentJob();
					work.reset();
				}
			}
		}
	};
}}}
