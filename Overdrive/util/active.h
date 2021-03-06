#pragma once

#include <memory>
#include <functional>
#include <type_traits>

#include <boost/thread.hpp>
#include <boost/thread/sync_queue.hpp>

namespace overdrive {
	namespace util {
		/**
		Based on Sutters / Kjellkod's code, this is an Active object
		Allows threadsafe execution of void() functions in an object (by internally queuing calls)
		The object maintains its own (OS-level!) thread, so don't overuse this

		[NOTE] MSVC has some kind of bug in the cleanup of global objects, which causes the destructor
		to contain some... questionable code to deal with it. It's the reason I'm using boost::thread
		instead of std::thread

		[NOTE] The boost::sync_queue is a candidate for replacement with moodycamel::concurrent_queue
			   Except for the fact that this is not intended for high-speed components :P
		*/
		typedef std::function<void()> Callback;

		class Active {
		private:
			Active(); //only allow creation via the factory method

		public:
			Active(const Active&) = delete;
			Active& operator = (const Active&) = delete;

			~Active();

			static std::unique_ptr<Active> create(); // factory method (make sure 

													 /**
													 In practice, call this with [=] lambda's -- by copying the data
													 we ensure that the data to be worked on is still live by the time
													 the active object gets to actually process it.

													 [NOTE] if the callback calls non-const methods the lambda will have to be mutable... -_-
													 */
			void send(Callback message);

		private:
			void run(); // thread method

			bool mIsDone;
			boost::sync_queue<Callback> mMessageQueue; // I'm assuming that the queue is mostly in a waiting state
			boost::thread mThread;
		};
	}
}