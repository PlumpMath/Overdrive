#include "taskprocessor.h"
#include "logger.h"

namespace overdrive {
	namespace core {
		TaskProcessor::TaskProcessor(size_t numWorkers):
			mIsRunning{ false }
		{
			if (numWorkers == 0) {
				mNumWorkers = boost::thread::hardware_concurrency();
				
				//make sure there is at least 1 worker, otherwise background tasks may not be executed at all
				if (mNumWorkers == 0)
					mNumWorkers = 1;
			}
			else
				mNumWorkers = numWorkers;
		}

		TaskProcessor::~TaskProcessor() {
			stop();

			mBackgroundWorkers.join_all();
		}

		void TaskProcessor::add(
			Task t, 
			bool repeating, 
			bool background
		) {
			add(
				make_wrapped(t, repeating, background)
			);
		}

		void TaskProcessor::add(detail::WrappedTask t) {
			if (t.isBackground())
				mBackgroundTasks.push(std::move(t));
			else
				mMainTasks.push(std::move(t));
		}

		void TaskProcessor::start() {
			mIsRunning = true;

			//start the workers
			for (size_t i = 0; i < mNumWorkers; ++i)
				mBackgroundWorkers.create_thread([this] {
					detail::WrappedTask task;
					
					while (mIsRunning) {
						mBackgroundTasks.pop(task);

						if (mIsRunning)
							execute(task);
					}
				});

			while (mIsRunning) {
				//create a snapshot of the main task queue and the synced tasks
				auto mainTasks = mMainTasks.toVectorAndClear();

				//fetch tasks and execute directly from the internal queue (no popping/locking required)
				for (const auto& task: mainTasks) 
					execute(task);
			}
		}

		void TaskProcessor::stop() {
			mIsRunning = false;
		}

		void TaskProcessor::execute(detail::WrappedTask t) {
			t();

			if (t.isRepeating())
				add(t);
		}
	}
}
