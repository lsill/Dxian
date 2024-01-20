//
// Created by lsill on 2024/1/20.
//

#ifndef DXIAN_SOCKET_H
#define DXIAN_SOCKET_H

#include <sys/socket.h>
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
};

class tcp_socket : public ISocket {
public:
    tcp_socket() {
        fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NOBLOCK, 0);
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
    }

    ssize_t receive(char* buffer, size_t length) override {
        // 接收数据...
    }


private:
    friend std::unique_ptr<tcp_socket> std::make_unique<tcp_socket, int&>(int&);
    explicit tcp_socket(int fd):fd_(fd), valid_(fd > 0) {}
    int fd_;
    int valid_;
};

#endif //DXIAN_SOCKET_H
