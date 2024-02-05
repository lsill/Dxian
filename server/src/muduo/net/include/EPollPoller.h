//
// Created by lsill on 2024/2/5.
//

#ifndef DXIAN_EPOLLPOLLER_H
#define DXIAN_EPOLLPOLLER_H

#include "Poller.h"

#include <vector>

#ifdef __linux__
struct epoll_event;
#endif
#ifdef __APPLE__
struct kevent;
#endif

namespace muduo
{
    namespace net
    {

///
/// IO Multiplexing with epoll(4).
///
        class EPollPoller : public Poller
        {
        public:
            EPollPoller(EventLoop* loop);
            ~EPollPoller() override;

            Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
            void updateChannel(Channel* channel) override;
            void removeChannel(Channel* channel) override;

        private:
            static const int kInitEventListSize = 16;

            static const char* operationToString(int op);

            void fillActiveChannels(int numEvents,
                                    ChannelList* activeChannels) const;
            void update(int operation, Channel* channel);
#if __linux__
            typedef std::vector<struct epoll_event> EventList;
#elif __APPLE__
            typedef std::vector<struct kevent> EventList;
#endif
            int epollfd_;
            EventList events_;
        };

    }  // namespace net
}  // namespace muduo




#endif //DXIAN_EPOLLPOLLER_H
