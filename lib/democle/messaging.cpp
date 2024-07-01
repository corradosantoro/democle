/*
 * messaging.cpp
 */

#include "democle.h"

MessageSender operator,(std::string dest, AtomicFormula b)
{
    //cout << "Sending message " << b << " from agent " << context->get_engine()->get_name() << " to agent " << destination << "," << a;
    return MessageSender(nullptr, dest, b);
    //b.set_sender(context->get_engine()->get_name());
    //(*a) + b;
    //return (*this);
}

// MessageSender & MessageSender::operator<<(AtomicFormula b)
// {
//     Agent * a = DEMOCLE::instance()->get_agent(destination);
//     if (a == nullptr)
//         throw AgentNotFoundException(destination);
//     if (!a->verify_message(b))
//        throw MessageNotAcceptedException();
//     //cout << "Sending message " << b << " from agent " << context->get_engine()->get_name() << " to agent " << destination << "," << a;
//     b.set_sender(context->get_engine()->get_name());
//     (*a) + b;
//     return (*this);
// }

Context & Context::operator<<(MessageSender msg)
{
    std::string destination = msg.get_destination();
    Agent * a = DEMOCLE::instance()->get_agent(destination);
    AtomicFormula & b = msg.get_belief();

    if (a == nullptr)
        throw AgentNotFoundException(destination);

    if (!a->verify_message(b))
       throw MessageNotAcceptedException();

    b.set_sender(this->get_engine()->get_name());
    (*a) + b;
    return (*this);
}

