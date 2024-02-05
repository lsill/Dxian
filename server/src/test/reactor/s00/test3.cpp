//
// Created by lsill on 2024/2/5.
//
#ifdef __linux__
#include "Channel.h"
#include "EventLoop.h"
#include "Timestamp.h"

#include <stdio.h>
#include <sys/timerfd.h>

muduo::net::EventLoop* g_loop;

void timeout(muduo::net::Timestamp receiveTime)
{
    printf("Timeout!\n");
    g_loop->quit();
}

int main()
{
    muduo::net::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::net::Channel channel(&loop, timerfd);
    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);
}
#endif

#ifdef __APPLE__
int main()
{

}
#endif