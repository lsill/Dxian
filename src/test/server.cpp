//
// Created by lsill on 2024/1/21.
//
#include "socket.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main() {
    cout << "hello world!\n ";
    ISocket* tcp_soc = new tcp_socket();
    tcp_soc->bind("127.0.0.1", 9534);
    tcp_soc->listen(10);
    while (true) {
        auto cli_soc = tcp_soc->accept();
        if (cli_soc) {
            cout<<"client\n";
        }
    }
    delete tcp_soc;
}