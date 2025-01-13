/*
 * protocol.h
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "democle.h"

class AbstractProtocol {
 public:
    AbstractProtocol() { };
    virtual void send_message(Agent * sender, url & destination, AtomicFormula & a) = 0;
};

class TCPProtocol : public AbstractProtocol {
    int port_number;
    string host;
    thread * tcp_thread;
    int server_fd;
 public:
    TCPProtocol(string _host,int port_num);
    virtual void send_message(Agent * sender, url & destination, AtomicFormula & a) override;
    void run();
};

#endif
