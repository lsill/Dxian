//
// Created by lsill on 2024/2/5.
//
#include "include/Poller.h"
#include "include/Channel.h"

using namespace muduo;
using namespace muduo::net;

Poller::Poller(muduo::net::EventLoop *loop):ownerLoop_(loop) {}

Poller::~Poller() =default;

bool Poller::hasChannel(muduo::net::Channel *channel) const {
    assertInLoopThread();
    ChannelMap::const_iterator it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}