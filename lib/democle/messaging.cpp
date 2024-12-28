/*
 * messaging.cpp
 */

#include "democle.h"
#include "url.h"

MessageSender operator,(std::string dest, AtomicFormula b)
{
    //cout << "Sending message to " << dest << endl;
    return MessageSender(nullptr, dest, b);
    //b.set_sender(context->get_engine()->get_name());
    //(*a) + b;
    //return (*this);
}

MessageSender operator,(char * dest, AtomicFormula b)
{
    return MessageSender(nullptr, dest, b);
}

MessageSender operator,(const char * dest, AtomicFormula b)
{
    return MessageSender(nullptr, dest, b);
}

Context & Context::operator+(MessageSender msg)
{
    std::string destination = msg.get_destination();

    url u(destination);

    cout << u.protocol << '\t' << u.host << "\t" << u.port << "\t" << u.path << endl;

    if (!u.is_good) {
        Agent * a = DEMOCLE::instance()->get_agent(destination);
        AtomicFormula & b = msg.get_belief();
        //cout << "Sending message from agent " << get_engine()->get_name() << " to agent " << destination << "," << a << endl;

        if (a == nullptr)
            throw AgentNotFoundException(destination);

        if (!a->verify_message(b))
            throw MessageNotAcceptedException();

        b.set_sender(this->get_engine()->get_name());
        (*a) + b;
    }
    else {
        cout << "Sending message from agent " << get_engine()->get_name() << " to remote agent " << destination << endl;
    }
    return (*this);
}

