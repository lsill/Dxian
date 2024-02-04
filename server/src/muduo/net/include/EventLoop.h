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

class EventLoop:noncopyable {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }
    EventLoop* getEventLoopOfCurrentThread();
    [[nodiscard]] bool isInLoopThread()const{return threadId_ == CurrentThread::tid();}
private:
    void abortNotInLoopThread();

    bool lopping_;
    const pid_t threadId_;
};


}

#endif //DXIAN_EVENTLOOP_H
