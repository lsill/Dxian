//
// Created by lsill on 2024/1/19.
//

#ifndef DXIAN_SERVERAPP_H
#define DXIAN_SERVERAPP_H
#include "socket_poll.h"
#include "tcp_socket.h"

#define MAX_EVENT 64


class ServerApp {
public:
    ServerApp():efd(-1){}
    virtual ~ServerApp() {
        if (efd != -1) {
            sp_release(efd);
        }
    }
    bool init();

    bool add_socket(int sock, void *ud) const {
        return sp_add(efd, sock, ud);
    }

    void remove_socket(int sock) const {
        sp_del(efd, sock);
    }

    void enable_socket(int sock, void* ud, bool b_read, bool b_write) const {
        sp_enable(efd, sock, ud, b_read, b_write);
    }

    [[noreturn]] void run();

private:
    int efd;
    std::unique_ptr<ISocket> socket;
};

#endif //DXIAN_SERVERAPP_H
