//
// Created by lsill on 2024/1/19.
//

#ifndef DXIAN_SERVERAPP_H
#define DXIAN_SERVERAPP_H
#include "socket_poll.h"

class ServerApp {
public:
    ServerApp():efd(-1){}
    virtual ~ServerApp() {
        if (efd != -1) {
            sp_release(efd);
        }
    }

    bool init() {
        efd = sp_create();
        return !sp_invalid(efd);
    }

    bool add_socket(int sock, void *ud) {
        return sp_add(efd, sock, ud);
    }

    void remove_socket(int sock) {
        sp_del(efd, sock);
    }

    void enable_socket(int sock, void* ud, bool b_read, bool b_write) {
        sp_enable(efd, sock, ud, b_read, b_write);
    }

    void run() {
        const int MAX_EVENTS = 1024;
        struct event events[MAX_EVENTS];

        while (true) {
            int n = sp_wait(efd, events, MAX_EVENTS);
            for (int i = 0; i < n;i++) {
                if (events[i].error || events[i].eof) {
                    // handle error or eof
                } else if (events[i].read){
                    // handle read
                } else if (events[i].write) {
                    // handle write
                }
            }
        }
    }

private:
    int efd;
};

#endif //DXIAN_SERVERAPP_H
