//
// Created by lsill on 2024/2/4.
//
#include "include/EventLoop.h"
#include "include/TimerId.h"
#include "Logging.h"

#include "poll.h"

using namespace muduo;
using namespace muduo::net;

thread_local EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop():lopping_(false),threadId_(CurrentThread::tid()) {
    LOG_TRACE << "EventLoop created" << this << " in thread" << threadId_;
    if (t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop "<< t_loopInThisThread << "exists in this thread" << threadId_;
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop() {
    // assert是一个宏，用于在运行时对表达式进行断言检查。如果表达式计算为假（即0），
    // assert 宏会导致程序终止，并打印一条错误消息，指出断言失败的文件名和行号。
    assert(!lopping_);
    t_loopInThisThread = nullptr;
}

EventLoop *EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}

void EventLoop::loop() {
    assert(!lopping_);
    assertInLoopThread();
    lopping_ = true;
    ::poll(nullptr, 0, 5*1000);

    LOG_TRACE<<"EventLoop "<< this << " stop looping";
    lopping_ = false;
}

void EventLoop::abortNotInLoopThread() {

}

void EventLoop::updateChannel(muduo::net::Channel *channel) {

}