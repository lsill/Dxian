//
// Created by lsill on 2024/2/6.
//

#ifndef DXIAN_EVENTLOOPTHREAD_H
#define DXIAN_EVENTLOOPTHREAD_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

namespace muduo
{
    namespace net
    {

        class EventLoop;

        class EventLoopThread : noncopyable
        {
        public:
            typedef std::function<void(EventLoop*)> ThreadInitCallback;

            EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                            const string& name = string());
            ~EventLoopThread();
            EventLoop* startLoop();

        private:
            void threadFunc();

            EventLoop* loop_ GUARDED_BY(mutex_);
            bool exiting_;
            Thread thread_;
            MutexLock mutex_;
            Condition cond_ GUARDED_BY(mutex_);
            ThreadInitCallback callback_;
        };

    }  // namespace net
}  // namespace muduo

#endif //DXIAN_EVENTLOOPTHREAD_H
