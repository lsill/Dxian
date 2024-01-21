//
// Created by lsill on 2024/1/20.
//

#ifndef DXIAN_SOCKET_H
#define DXIAN_SOCKET_H

#include <netdb.h> //提供了网络数据库操作的函数，如获取主机名、服务名等。常用于解析域名和获取主机信息。
#include <fcntl.h>  // 提供了对文件描述符的控制操作，如改变已打开的文件属性。常用于设置套接字为非阻塞模式
#include <sys/epoll.h>  //  Linux 的 epoll 接口，提供了高效的多路复用 IO。用于同时处理多个网络连接，比如在服务器端监听多个客户端请求。
#include <sys/types.h> // 定义了一些基本的数据类型，如 pid_t（用于进程 ID）、ssize_t（用于系统调用的返回类型）等。
#include <sys/socket.h> // 提供了套接字接口，用于创建套接字、绑定、监听、接收和发送数据等。这是网络编程中最核心的头文件之一。
#include <netinet/in.h> // 定义了互联网协议族的数据结构和操作函数，如 IP 地址和端口号的结构体（如 sockaddr_in）。
#include <arpa/inet.h>   // 提供了用于操作网络地址的函数，如 IP 地址转换函数（如 inet_pton 和 inet_ntop）。
#include <string>
#include <memory>

class ISocket {
public:
    virtual ~ISocket() = default;
    virtual void bind(const std::string& host, int port) = 0;
    virtual void listen(int backlog) = 0;
    virtual std::unique_ptr<ISocket> accept() = 0;
    virtual void connect(const std::string& host, int port) = 0;
    virtual ssize_t send(const char* buffer, size_t length) = 0;
    virtual ssize_t receive(char* buffer, size_t length) = 0;
    virtual bool valid() const = 0;
    virtual int fd() const = 0;
};

class tcp_socket : public ISocket {
public:
    tcp_socket() {
        fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        valid_ = fd_ > 0;
    }

    void bind(const std::string& host, int port) override {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(host.c_str());
        addr.sin_port = htons(port);
        ::bind(fd_, (struct sockaddr*)&addr,sizeof(addr));
    }

    void listen(int backlog) override {
        ::listen(fd_, backlog);
    }

    std::unique_ptr<ISocket> accept() override {
        int c_fd = ::accept(fd_, nullptr, nullptr);
        if (c_fd <= 0) {
            // TODO
            return nullptr;
        }
        return std::make_unique<tcp_socket>(c_fd);
    }

    void connect(const std::string& host, int port) override {

    }

    ssize_t send(const char* buffer, size_t length) override {
        // 发送数据...
        return 0;
    }

    ssize_t receive(char* buffer, size_t length) override {
        // 接收数据...
        return 0;
    }

    bool valid() const override {return valid_;}
    int fd() const override{return fd_;}

private:
    friend std::unique_ptr<tcp_socket> std::make_unique<tcp_socket, int&>(int&);
    explicit tcp_socket(int fd):fd_(fd), valid_(fd > 0) {}
    int fd_;
    int valid_;
};

#endif //DXIAN_SOCKET_H
