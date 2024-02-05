//
// Created by lsill on 2024/2/5.
//
#include "Logging.h"
#include "Types.h"

#include "include/PollPoller.h"
#include "include/Channel.h"

#include <cassert>
#include <cerrno>
#include <poll.h>

using namespace muduo;
using namespace muduo::net;
// TODO 这里的assert可能要做个错误处理
PollPoller::PollPoller(EventLoop* loop)
        : Poller(loop)
{
}

PollPoller::~PollPoller() = default;

Timestamp PollPoller::poll(int timeoutMs, ChannelList *activeChannels) {
    // XXX pollfds_ shouldn't change
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
    int savedErrno = errno;
    Timestamp now(Timestamp::now());
    if (numEvents > 0) {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
    } else if (numEvents == 0) {
        LOG_TRACE << " nothing happened";
    } else {
        if (savedErrno != EINTR)
        {
            errno = savedErrno;
            LOG_SYSERR << "PollPoller::poll()";
        }
    }
    return now;
}

void PollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const {
    for (auto pfd = pollfds_.begin();
    pfd != pollfds_.end();++pfd) {
        if (pfd->revents > 0) {
            --numEvents;
            auto ch = channels_.find(pfd->fd);
            assert(ch != channels_.end());
            Channel* channel = ch->second;
            assert(channel->fd() == pfd->fd);
            channel->set_revents(pfd->revents);
            // pfd->revents = 0;
            activeChannels->push_back(channel);
        }
    }
}

void PollPoller::updateChannel(muduo::net::Channel *channel) {
    Poller::assertInLoopThread();
    LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
    if (channel->index() < 0) {
        // a new one, add to pollfds_
        assert(channels_.find(channel->fd()) == channels_.end()); // 找到了退出
        struct pollfd pfd{};    // 新建poll
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);    // 加入poll列表
        int idx = static_cast<int>(pollfds_.size())-1;
        channel->set_index(idx);    // 设置channel在列表的位置
        channels_[pfd.fd] = channel;    // map中加映射
    } else {
        // update existing one
        assert(channels_.find(channel->fd()) != channels_.end());   // 没找到的话退出
        assert(channels_[channel->fd()] == channel);    // map中文件描述符对应的不一定退出
        int idx = channel->index();
        assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));    // 不在列表区间退出，主要是为了下面的防越界
        struct pollfd& pfd = pollfds_[idx]; // 找到poll
        assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() -1); // 判断 文件描述符是否一致
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if (channel->isNoneEvent())
        {
            // ignore this pollfd
            pfd.fd = -channel->fd()-1;
        }
    }
}

void PollPoller::removeChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    LOG_TRACE << "fd = " << channel->fd();
    assert(channels_.find(channel->fd()) != channels_.end());   // 没找到描述符退出
    assert(channels_[channel->fd()] == channel);    // 找到了但不是一个channel
    assert(channel->isNoneEvent()); // 还有事件没处理完退出
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size())); // 越界判定
    const struct pollfd& pfd = pollfds_[idx]; (void)pfd;    // 避免未使用变量警告
    assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
    size_t n = channels_.erase(channel->fd());
    assert(n == 1); (void)n;
    // implicit_cast 仅允许那些本来就能通过编译器的隐式转换规则进行的转换
    if (implicit_cast<size_t>(idx) == pollfds_.size()-1)
    {
        pollfds_.pop_back();
    }
    else
    {
        int channelAtEnd = pollfds_.back().fd;
        iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
        if (channelAtEnd < 0)
        {
            channelAtEnd = -channelAtEnd-1;
        }
        channels_[channelAtEnd]->set_index(idx);
        pollfds_.pop_back();
    }
}
