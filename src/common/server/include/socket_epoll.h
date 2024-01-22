//
// Created by lsill on 2024/1/19.
//

#ifndef DXIAN_SOCKET_EPOLL_H
#define DXIAN_SOCKET_EPOLL_H

#ifdef __linix__

#include <netdb.h> //提供了网络数据库操作的函数，如获取主机名、服务名等。常用于解析域名和获取主机信息。
#include <fcntl.h>  // 提供了对文件描述符的控制操作，如改变已打开的文件属性。常用于设置套接字为非阻塞模式
#include <sys/epoll.h>  //  Linux 的 epoll 接口，提供了高效的多路复用 IO。用于同时处理多个网络连接，比如在服务器端监听多个客户端请求。
#include <sys/types.h> // 定义了一些基本的数据类型，如 pid_t（用于进程 ID）、ssize_t（用于系统调用的返回类型）等。
#include <sys/socket.h> // 提供了套接字接口，用于创建套接字、绑定、监听、接收和发送数据等。这是网络编程中最核心的头文件之一。
#include <netinet/in.h> // 定义了互联网协议族的数据结构和操作函数，如 IP 地址和端口号的结构体（如 sockaddr_in）。
#include <arpa/inet.h>   // 提供了用于操作网络地址的函数，如 IP 地址转换函数（如 inet_pton 和 inet_ntop）。
#include "socket_poll.h"

static bool
sp_invalid(int efd){
    return efd == -1;
}

static int
sp_create() {
    return epoll_create(1024);
}

static void
sp_release(int efd) {
    close(efd);
}

/*struct epoll_event {
    uint32_t     events;    // EPOLLIN（表示对应的文件描述符可读）、EPOLLOUT（可写）、EPOLLPRI（有紧急数据可读）、EPOLLERR（错误发生）、EPOLLHUP（挂起事件）
    epoll_data_t data;
};*/

/*typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;*/

static int
sp_add(int efd, int sock, void* ud) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ud;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        return 1;
    }
    return 0;
}

static void
sp_del(int efd, sock) {
    epoll_ctl(efd, EPOLL_CTL_DEL, sock, nullptr);
}

static int
sp_enable(int efd, int sock, void* ud, bool b_read, bool b_write) {
    struct epoll_event ev;
    ev.events = (b_read ? EPOLLIN : 0) | (b_write ? EPOLLOUT  : 0);
    ev.data.ptr = ud;
    if (epoll_ctl(efd, EPOLL_CTL_MOD, sock, &ev) == -1) {
        return 1;
    }
    return 0;
}

static int
sp_wait(int efd, struct event *e, int max) {
    struct epoll_event ev[max];
    int n = epoll_wait(efd, ev, max, -1);
    int i;
    for (i = 0; i < n;i++) {
        e[i].s = ev[i].data.ptr;
        unsigned flag = ev[i].events;
        e[i].write = (flag & EPOLLOUT) != 0;
        e[i].read = (flag & EPOLLIN) != 0;
        e[i].error = (flag & EPOLLERR) != 0;
        e[i].eof = (flag & EPOLLHUP) != 0;
    }
    return n;
}

#endif

#endif //DXIAN_SOCKET_EPOLL_H
