/*
 * democle.cpp
 */

#include "democle.h"

DEMOCLE * DEMOCLE::_instance = nullptr;

DEMOCLE::DEMOCLE()
{
}

void DEMOCLE::register_agent(Agent * a)
{
    registry[a->get_name()] = a;
}

Agent * DEMOCLE::get_agent(string name)
{
    if (registry.count(name))
        return registry[name];
    else
        return nullptr;
}

void DEMOCLE::_register_tcp_protocol(va_list args)
{
    int port = va_arg(args, int);
    protocol_registry["tcp"] = new TCPProtocol(port);
}

void DEMOCLE::_send_message(url & destination, AtomicFormula & b)
{
    if (protocol_registry.count(destination.protocol)) {
        AbstractProtocol * p = protocol_registry[destination.protocol];
        p->send_message(destination, b);
    }
    else
        throw new ProtocolNotStartedException();
}


