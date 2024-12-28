/*
 * protocol.h
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "democle.h"

class AbstractProtocol {
 public:
    AbstractProtocol() { };
    virtual void send_message(std::string & destination, AtomicFormula & a) = 0;
};

class TCPProtocol : public AbstractProtocol {
    int port_number;
    thread * tcp_thread;
 public:
    TCPProtocol(int port_num);
    virtual void send_message(std::string & destination, AtomicFormula & a) override;
    void run();
};

#endif
