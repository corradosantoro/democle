/*
 * messaging.cpp
 */

#include "democle.h"

MessageSender & MessageSender::operator<<(AtomicFormula b)
{
    Agent * a = DEMOCLE::instance()->get_agent(destination);
    if (a == nullptr)
        throw AgentNotFoundException(destination);
    if (!a->verify_message(b))
       throw MessageNotAcceptedException();
    //cout << "Sending message " << b << " from agent " << context->get_engine()->get_name() << " to agent " << destination << "," << a;
    b.set_sender(context->get_engine()->get_name());
    (*a) + b;
    return (*this);
}

