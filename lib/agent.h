/*
 * agent.h
 */

#ifndef __AGENT_H
#define __AGENT_H

#include <string>
#include "engine.h"

using namespace std;

#define accepts(...) accept_messages(0, #__VA_ARGS__, nullptr)
#define attach(s) { s->set_agent(this); sensors.push_back(s); }

class Sensor;

class Agent {
 public:
    Agent(string _name);
    void start();
    virtual void run() = 0;
    void show_plans() { e->show_plans(); };
    void show_kb() { e->show_kb(); };
    string & get_name() { return name; };
    Engine & operator+(AtomicFormula b);
    Engine & operator-(AtomicFormula b);
    Engine & operator<<(AtomicFormula b);
    bool verify_message(AtomicFormula & b);
    vector<Sensor *> & get_sensors() { return sensors; };

    friend Engine & operator+(Agent * a, AtomicFormula b) { return (*a) + b; };
    friend Engine & operator-(Agent * a, AtomicFormula b) { return (*a) - b; };
    friend Engine & operator<<(Agent * a, AtomicFormula b) { return (*a) << b; };
 protected:
    void accept_messages(int x,...);
    vector<Sensor *> sensors;
 private:
    Engine * e;
    string name;
    vector<pair<string, int>> accepted_messages;
};

#endif
