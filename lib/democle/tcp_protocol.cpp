/*
 * tcp_protocol.cpp
 */

#include "democle.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


void tcp_thread_start(TCPProtocol * p)
{
    p->run();
}


TCPProtocol::TCPProtocol(int port_num)
    : AbstractProtocol(), port_number(port_num)
{
    tcp_thread = new thread(tcp_thread_start, this);
    tcp_thread->detach();
}

void TCPProtocol::run()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_number);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("accept");
            continue;
        }
        // ssize_t valread = read(new_socket, buffer, BUFFER_SIZE);
        // std::cout << "Received: " << buffer << std::endl;
        // send(new_socket, buffer, valread, 0);
        // std::cout << "Echo message sent" << std::endl;
        close(new_socket);
    }
}

void TCPProtocol::send_message(std::string & destination, AtomicFormula & a)
{
}

