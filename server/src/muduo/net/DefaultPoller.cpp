//
// Created by lsill on 2024/2/5.
//
#include "include/Poller.h"
#include "include/PollPoller.h"
#include "include/EPollPoller.h"

#include <stdlib.h>

using namespace muduo::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if (::getenv("MUDUO_USE_POLL"))
    {
        return new PollPoller(loop);
    }
    else
    {
#if __linux__
        return new EPollPoller(loop);
#elif __APPLE__

#endif
    }
}