//
// Created by lsill on 2024/1/22.
//
#include "tcp_socket.h"
#include <unistd.h>

tcp_socket::tcp_socket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1) {
        valid_ = false;
        return;
    }
    // 设置套接字为非阻塞模式
    int flags = fcntl(fd_, F_GETFL, 0);
    if (flags == -1) {
        close(fd_);
        valid_ = false;
        return;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd_, F_SETFL, flags) == -1) {
        // 处理 fcntl 错误
        close(fd_);  // 注意关闭套接字
        valid_ = false;
        return;
    }
    valid_ = fd_ >= 0;
}