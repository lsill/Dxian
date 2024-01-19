//
// Created by 92486 on 2024/1/19.
//

#ifndef DXIAN_SOCKET_EPOLL_H
#define DXIAN_SOCKET_EPOLL_H

#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>

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

}

#endif //DXIAN_SOCKET_EPOLL_H
