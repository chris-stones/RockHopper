#pragma once

#include <thread>
#include <atomic>
#include <deque>
#include <vector>
#include <condition_variable>

#include "IConcurrentUpdatable.hpp"
#include "IThreadPool.hpp"

namespace RH { namespace Libs { namespace Concurrency {

	class IThreadPool {
	public:
		virtual ~IThreadPool() {}
		virtual void AddWork(std::weak_ptr<IConcurrentUpdatable> work) = 0;
	};

	class ThreadPool : public IThreadPool {

		typedef std::vector< std::thread> ThreadVector;
		typedef std::deque< std::weak_ptr<IConcurrentUpdatable> > WorkDeque;

		ThreadVector 	threadVector;
		WorkDeque 		workDeque;
		const int		max_queue;
		bool		 	shutdown_flag {false};

		std::mutex mutex;
		std::condition_variable readable;

		std::shared_ptr<IConcurrentUpdatable> GetWork() {

			std::unique_lock<std::mutex> lock( mutex );

			while( !shutdown_flag && ( workDeque.size() == 0) )
				readable.wait( lock );

			std::shared_ptr<IConcurrentUpdatable> work;

			if(workDeque.size()) {

				work = workDeque.front().lock();

				workDeque.pop_front();
			}

			return work;
		}

	public:

		ThreadPool(int threads)
			:	max_queue(threads)
		{
			threadVector.reserve(threads);
			for(int i=0;i<threads; i++)
				threadVector.push_back( std::thread( &ThreadPool::Main, this ) );
		}

		virtual ~ThreadPool() {

			{
				std::unique_lock<std::mutex> lock( mutex );
				shutdown_flag = true;
				readable.notify_all();
			}
			for(int i=0;i<threadVector.size(); i++)
				threadVector[i].join();
		}

		virtual void AddWork(std::weak_ptr<IConcurrentUpdatable> work) override {

			std::unique_lock<std::mutex> lock( mutex );
			workDeque.push_back(work);
			readable.notify_all();
		}

	private:

		void Main() {

			std::shared_ptr<IConcurrentUpdatable> work;

			while(!this->shutdown_flag) {

				if((work = GetWork())) {
					work->ConcurrentUpdate();
					work.reset();
				}
			}
		}
	};
}}}
