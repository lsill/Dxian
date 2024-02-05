//
// Created by lsill on 2024/2/5.
//
#include "Logging.h"
#include "include/EventLoop.h"
#include "include/Channel.h"

#include <sstream>
#ifdef __linux__
#define _GNU_SOURCE
#endif
#include "poll.h"

using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd__)
        : loop_(loop),
          fd_(fd__),
          events_(0),
          revents_(0),
          index_(-1),
          logHup_(true),
          tied_(false),
          eventHandling_(false),
          addedToLoop_(false)
{
}

Channel::~Channel()
{
    assert(!eventHandling_);
    assert(!addedToLoop_);
    if (loop_->isInLoopThread())
    {
        //assert(!loop_->hasChannel(this));
    }
}

void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::handleEvent(muduo::Timestamp receiveTime) {
    if (revents_ & POLLNVAL) {
        LOG_WARN << "Channel::handle_event() POLLNVAL";
    }
    if (revents_ & (POLLERR | POLLNVAL)) {
        if (errorCallback_) errorCallback_();
    }

#if __linux__
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUB)) {
#elif __APPLE__
    if (revents_ & (POLLIN | POLLPRI )) {
#endif
        if (readCallback_) readCallback_(Timestamp());  // TODO 这里需要传参
    }
    if (revents_ & POLLOUT) {
        if (writeCallback_) writeCallback_();
    }
}