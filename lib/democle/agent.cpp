/*
 * agent.cpp
 */

#include "democle.h"
#include <cstdarg>

Agent::Agent(string _name) : name(_name)
{
    e = new Engine(this, name);
    DEMOCLE::instance()->register_agent(this);
}

void Agent::start()
{
    Engine::set_current(e);
    run();
    e->start();
}

Engine & Agent::operator+(AtomicFormula b)
{
    //std::cout << "BEGIN Agent Operator +" << std::endl;
    (*e) + b;
    //std::cout << "END Agent Operator +" << std::endl;
    return *e;
}


Engine & Agent::operator-(AtomicFormula b)
{
    //std::cout << "BEGIN Agent Operator -" << std::endl;
    (*e) - b;
    //std::cout << "END Agent Operator -" << std::endl;
    return *e;
}

Engine & Agent::operator<<(AtomicFormula b)
{
    (*e) << b;
    return *e;
}


bool Agent::verify_message(AtomicFormula & b)
{
    for (auto it = accepted_messages.begin(); it != accepted_messages.end(); it++) {
        if (it->first == b.get_name() && it->second == b.arity())
            return true;
    }
    return false;
}


void Agent::accept_messages(int x,...)
{
    va_list args;
    va_start(args, x);

    const char * c = va_arg(args, const char *);

    while (c != nullptr) {
        string s = c;
        int pos;
        if ( (pos = s.find("/")) == string::npos)
            throw BadMessageSyntaxException(s);
        string bel_name = s.substr(0, pos);
        int arity = stoi(s.substr(pos+1));
        accepted_messages.push_back(make_pair(bel_name, arity));
        c = va_arg(args, const char *);
    }

    va_end(args);
}

