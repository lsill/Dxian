//
// Created by lsill on 2024/1/20.
//

#include "serverapp.h"
#include "tcp_socket.h"

bool ServerApp::init() {
    efd = sp_create();
    if (sp_invalid(efd)) {
        return false;
    }
    socket = std::make_unique<tcp_socket>();
    socket->bind("127.0.0.1", 9546);
    int ret = sp_add(efd, socket->fd(), socket.get());
    if (ret == -1) {
        return false;
    }
    return true;
}

[[noreturn]] void ServerApp::run() {
    struct event events[MAX_EVENT];

    while (true) {
        int n = sp_wait(efd, events, MAX_EVENT);
        for (int i = 0; i < n;i++) {
            if (events[i].error || events[i].eof) {
                // handle error or eof
                break;
            } else if (events[i].read){
                // handle read
            } else if (events[i].write) {
                // handle write
            }
        }
    }
}