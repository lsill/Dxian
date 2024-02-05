//
// Created by lsill on 2024/2/5.
//

#ifndef DXIAN_POLLPOLLER_H
#define DXIAN_POLLPOLLER_H

#include "Poller.h"

struct pollfd;

namespace muduo{
namespace net {
class PollPoller: public Poller{
public:
    PollPoller(EventLoop* loop);
    ~PollPoller() override;
    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;
private:
    void fillActiveChannels(int numEvents,
                            ChannelList* activeChannels) const;
    typedef std::vector<struct pollfd> PollFdList;
    PollFdList pollfds_;
};
}
}

#endif //DXIAN_POLLPOLLER_H
