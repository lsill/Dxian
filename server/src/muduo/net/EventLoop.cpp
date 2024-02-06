//
// Created by lsill on 2024/2/4.
//
#include "include/EventLoop.h"
#include "include/TimerId.h"
#include "include/Poller.h"
#include "include/Channel.h"
#include "include/TimerQueue.h"
#include "CurrentThread.h"
#include "Logging.h"

#include "poll.h"
#include <sys/eventfd.h>

using namespace muduo;
using namespace muduo::net;

thread_local EventLoop* t_loopInThisThread = nullptr;
const int kPollTimeMs = 10000;

int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0)
    {
        LOG_SYSERR << "Failed in eventfd";
        abort();
    }
    return evtfd;
}


EventLoop::EventLoop()
        : looping_(false),
          quit_(false),
          eventHandling_(false),
          callingPendingFunctors_(false),
          iteration_(0),
          threadId_(CurrentThread::tid()),
          poller_(Poller::newDefaultPoller(this)),
          timerQueue_(new TimerQueue(this)),
          wakeupFd_(createEventfd()),
          wakeupChannel_(new Channel(this, wakeupFd_)),
          currentActiveChannel_(nullptr)
{
    LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
                  << " exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(
            std::bind(&EventLoop::handleRead, this));
    // we are always reading the wakeupfd
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << CurrentThread::tid();
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);
    t_loopInThisThread = nullptr;
}
EventLoop *EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while (!quit_) {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        for (auto & activeChannel : activeChannels_) {
            activeChannel->handleEvent(Timestamp::now());
        }
    }

    LOG_TRACE<<"EventLoop "<< this << " stop looping";
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
    return timerQueue_->addTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, std::move(cb));
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(std::move(cb), time, interval);
}


void EventLoop::runInLoop(muduo::net::EventLoop::Functor cb) {
    if (isInLoopThread())
    {
        cb();
    }
}

void EventLoop::abortNotInLoopThread() {

}

void EventLoop::updateChannel(muduo::net::Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}