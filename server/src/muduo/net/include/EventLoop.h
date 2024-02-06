//
// Created by lsill on 2024/2/4.
//

#ifndef DXIAN_EVENTLOOP_H
#define DXIAN_EVENTLOOP_H

#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>

#include "noncopyable.h"
#include "Mutex.h"
#include "Timestamp.h"
#include "TimerId.h"
#include "Callbacks.h"

namespace muduo{
namespace net {
class Channel;
class Poller;
class TimerQueue;
    class EventLoop : noncopyable {
    public:
        typedef std::function<void()> Functor;

        EventLoop();

        ~EventLoop();

        void loop();
        void quit();

        // timers

        ///
        /// Runs callback at 'time'.
        /// Safe to call from other threads.
        ///
        TimerId runAt(Timestamp time, TimerCallback cb);
        ///
        /// Runs callback after @c delay seconds.
        /// Safe to call from other threads.
        ///
        TimerId runAfter(double delay, TimerCallback cb);
        ///
        /// Runs callback every @c interval seconds.
        /// Safe to call from other threads.
        ///
        TimerId runEvery(double interval, TimerCallback cb);

        /// Runs callback immediately in the loop thread.
        /// It wakes up the loop, and run the cb.
        /// If in the same loop thread, cb is run within the function.
        /// Safe to call from other threads.
        void runInLoop(Functor cb);

        void assertInLoopThread() {
            if (!isInLoopThread()) {
                abortNotInLoopThread();
            }
        }

        EventLoop *getEventLoopOfCurrentThread();

        [[nodiscard]] bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

        void updateChannel(Channel* channel);

    private:
        void abortNotInLoopThread();
        void handleRead();  // waked up
        void doPendingFunctors();

        void printActiveChannels() const; // DEBUG

        typedef std::vector<Channel*> ChannelList;

        bool looping_; /* atomic */
        std::atomic<bool> quit_;
        bool eventHandling_; /* atomic */
        bool callingPendingFunctors_; /* atomic */
        int64_t iteration_;
        const pid_t threadId_;
        Timestamp pollReturnTime_;
        std::unique_ptr<Poller> poller_;
        std::unique_ptr<TimerQueue> timerQueue_;
        int wakeupFd_;
        // unlike in TimerQueue, which is an internal class,
        // we don't expose Channel to client.
        std::unique_ptr<Channel> wakeupChannel_;
        boost::any context_;

        // scratch variables
        ChannelList activeChannels_;
        Channel* currentActiveChannel_;

        mutable MutexLock mutex_;
        std::vector<Functor> pendingFunctors_ GUARDED_BY(mutex_);
    };

}//namespace  net
}// namespace muduo

#endif //DXIAN_EVENTLOOP_H
