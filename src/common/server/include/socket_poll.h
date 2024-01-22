//
// Created by lsill on 2024/1/19.
//

#ifndef DXIAN_SOCKET_POLL_H
#define DXIAN_SOCKET_POLL_H

// #include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

typedef int poll_fd;

struct event {
    void* s;
    bool read;
    bool write;
    bool error;
    bool eof;
};

static bool sp_invalid(poll_fd fd);
static poll_fd sp_create();
static void sp_release(poll_fd fd);
static int sp_add(poll_fd fd, int sock, void* ud);
static void sp_del(poll_fd fd, int sock);
static int sp_enable(poll_fd, int sock, void *ud, bool read_enable, bool write_enable);
static int sp_wait(poll_fd, struct event* e, int max);
static void sp_nonblocking(int sock);


#ifdef __linux__
#include "socket_epoll.h"
#elif defined(__APPLE__)
#include "socket_kqueue.h"
#endif

#endif //DXIAN_SOCKET_POLL_H
