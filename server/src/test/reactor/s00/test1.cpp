//
// Created by lsill on 2024/2/4.
//
#include "EventLoop.h"
#include "Thread.h"
#include <cstdio>

void threadFunc()
{
    printf("threadFunc(): pid = %d, tid = %d\n",
           getpid(), muduo::CurrentThread::tid());

    muduo::net::EventLoop loop;
    loop.loop();
}

int main()
{
    printf("main(): pid = %d, tid = %d\n",
           getpid(), muduo::CurrentThread::tid());

    muduo::net::EventLoop loop;

    muduo::Thread thread(threadFunc);
    thread.start();

    loop.loop();
    pthread_exit(nullptr);
    return 0;
}
