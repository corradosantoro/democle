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

void DEMOCLE::_send_message(Agent * sender, url & destination, AtomicFormula & b)
{
    if (protocol_registry.count(destination.protocol)) {
        AbstractProtocol * p = protocol_registry[destination.protocol];
        p->send_message(sender, destination, b);
    }
    else
        throw new ProtocolNotStartedException();
}

void DEMOCLE::_put_message_in_queue(string & destination_agent, string & sender_agent, AtomicFormula & b)
{
    Agent * a = DEMOCLE::instance()->get_agent(destination_agent);

    if (a == nullptr)
        throw AgentNotFoundException(destination_agent);

    //cout << "Sending message from agent " << sender_agent << " to agent " << a->get_name() << "," << b << endl;
    if (!a->verify_message(b))
        throw MessageNotAcceptedException();

    b.set_sender(sender_agent);
    (*a) + b;
}



