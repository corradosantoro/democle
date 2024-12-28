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
    cout << "SONO QUI" << endl;
    for (std::vector<t_message_template>::iterator it = accepted_messages.begin(); it != accepted_messages.end(); it++) {
        t_message_template mt = *it;
        cout << "AM: " << mt.name << "," << mt.arity << endl;

        //if ( (*it) == b.get_name() ) //&& mt->arity == b.arity())
        //     return true;
    }
    return true;
}


void Agent::accept_messages(int x,...)
{
    va_list args;
    std::string bel_name;
    va_start(args, x);

    const char * c = va_arg(args, const char *);

    while (c != nullptr) {
        string s = c;
        int pos;
        if ( (pos = s.find("/")) == string::npos)
            throw BadMessageSyntaxException(s);
        bel_name = s.substr(0, pos);
        int arity = stoi(s.substr(pos+1));
        t_message_template mt;
        strcpy(mt.name,bel_name.c_str());
        mt.arity = arity;
        accepted_messages.push_back(mt);
        c = va_arg(args, const char *);
    }

    va_end(args);

    // for (auto it = accepted_messages.begin(); it != accepted_messages.end(); it++) {
    //     t_message_template mt = *it;
    //     cout << mt.name << "," << mt.arity << endl;
    // }
    // for (auto it = accepted_message_names.begin(); it != accepted_message_names.end(); it++) {
    //     cout << (*it) << endl;
    // }
}

