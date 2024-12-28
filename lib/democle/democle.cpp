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

