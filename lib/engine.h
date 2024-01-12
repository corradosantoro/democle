/*
 * engine.h
 */

#ifndef __ENGINE_H
#define __ENGINE_H

#include <thread>
#include "democle_types.h"
#include "goal.h"
#include "plan.h"
#include "tsqueue.h"

class Agent;

class KnowledgeBase {
    std::map<string, vector<AtomicFormula *>> kb;
    std::mutex m_mutex;
 public:
    KnowledgeBase() { };
    bool assert_belief(AtomicFormula * bel);
    bool retract_belief(AtomicFormula * bel);
    vector<AtomicFormula *> * get_named_beliefs(string & name);
    bool get_matching_beliefs(AtomicFormula * bel, Context * starting_context, ContextPtrVector * ctx_array);
    bool verify_matching_beliefs(Context * ctx, AtomicFormula * bel);
    void show();
};


class Engine {
    KnowledgeBase kb;
    static Engine * current;
    vector<Plan*> plans;
    vector<Goal*> goals;
    TSQueue<Event*> event_queue;
    thread * main_thread;
    string name;
    Agent * agent;
    void run();
 public:
    Engine(Agent * a) {
        agent = a;
        current = this;
        name = to_string((long)this);
    };
    Engine(Agent * a, string _nam) {
        agent = a;
        current = this;
        name = _nam;
    };
    Engine & operator+(AtomicFormula b);
    Engine & operator-(AtomicFormula b);
    Engine & operator<<(AtomicFormula b);
    void add_plan(Plan * p) { plans.push_back(p); };
    void add_goal(Goal * p) { goals.push_back(p); };
    void show_kb() { kb.show(); };
    void show_plans();
    void start();
    void execute_event(Event * evt);

    string & get_name() { return name; };

    Agent * get_agent() { return agent; };

    KnowledgeBase * knowledge() { return &kb;};
    static Engine & get_current() { return *current; };
    static void set_current(Engine * e) { current = e; };

    friend void thread_start(Engine * e);

};



#endif

