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
    class EventLoop : noncopyable {
    public:
        EventLoop();

        ~EventLoop();

        void loop();

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
        typedef std::vector<Channel*> ChannelList;

        bool lopping_;  // *atomic
        bool quit_; // *atomic
        const pid_t threadId_;
        std::unique_ptr<Poller> poller_;
        ChannelList activeChannels_;
    };

}//namespace  net
}// namespace muduo

#endif //DXIAN_EVENTLOOP_H
