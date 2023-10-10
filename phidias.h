/*
 * phidias.h
 */

#ifndef __PHIDIAS_H
#define __PHIDIAS_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include "tsqueue.h"

using namespace std;

class flexi_type {
protected:
    int value;
    float fvalue;
    string svalue;
public:
    flexi_type() : _type(no_type) { };
    flexi_type(int v) : value(v), _type(_int) { };
    flexi_type(float v) : fvalue(v), _type(_float) { };
    flexi_type(string v) : svalue(v), _type(_string) { };
    flexi_type(const char * v) : svalue(v), _type(_string) { };

    virtual operator int() const { return value; };
    virtual operator string() const { return svalue; };
    virtual operator float() const { return fvalue; };

    virtual void set(int val) { value = val; _type = _int; bound = true; };
    virtual void set(float val) { fvalue = val; _type = _float; bound = true; };
    virtual void set(const char * val) { svalue = val; _type = _string; bound = true; };
    virtual void set(string val) { svalue = val; _type = _string; bound = true; };

    virtual void set(flexi_type & t);

    virtual bool operator!=(flexi_type & t) { return !(*this == t); };
    virtual bool operator==(flexi_type & t);

    enum { no_type, _int, _float, _string } _type;
};

class term {
protected:
    int value;
    float fvalue;
    string svalue;
    bool bound;
    bool variab;
    string name;
public:
    term() : _type(no_type), bound(false), variab(false) { };
    term(int v) : value(v), _type(_int), bound(true), variab(false) { };
    term(float v) : fvalue(v), _type(_float), bound(true), variab(false) { };
    term(string v) : svalue(v), _type(_string), bound(true), variab(false) { };
    term(const char * v) : svalue(v), _type(_string), bound(true), variab(false) { };
    term(bool v, const char * n) : _type(no_type), bound(false), variab(true), name(n) { };
    virtual bool is_const() { return !variab; };
    virtual bool is_bound() {return bound;};
    virtual bool is_var() {return variab;};
    virtual void unbind() { bound = false; };
    virtual operator int() const { return value; };
    virtual operator string() const { return svalue; };
    virtual operator float() const { return fvalue; };

    virtual void set(int val) { value = val; _type = _int; bound = true; };
    virtual void set(float val) { fvalue = val; _type = _float; bound = true; };
    virtual void set(const char * val) { svalue = val; _type = _string; bound = true; };
    virtual void set(string val) { svalue = val; _type = _string; bound = true; };

    virtual void set(term & t);

    virtual bool operator!=(term & t) { return !(*this == t); };
    virtual bool operator==(term & t);

    string & get_name() { return name; };

    enum { no_type, _int, _float, _string } _type;
};


// class variable : public term {
// public:
//     variable(string _name) : term(), name(_name) { };
//     virtual bool is_const() { return false; };
//     virtual bool is_var() {return true;};
//     virtual void unbind() { bound = false; };
//     string & get_name() { return name; };
// private:
//     string name;
// };

class Engine;
class Belief;

class Context{
    map<string, term> variables;
    Engine * engine;
public:
    Context() : engine(nullptr) { };
    void set_engine(Engine * e) { engine = e; };
    term & operator[](string index) { return variables[index]; };
    Context & operator+(Belief b);
    //void operator[](string index, term & val) { variables[string] = val };

};



typedef vector<term> term_vector;

term_vector & mk_term(term_vector & terms);
template <typename H, typename... T> term_vector & mk_term(term_vector & terms, H head, T... tail)
{
    terms.push_back(head);
    return mk_term(terms, tail...);
}

class Belief;
class Event;

typedef function<void(Context & ctx)> action_fun;


typedef enum  { add, del } event_type;


class Plan {
    Event * event;
    action_fun fun;
public:
    Plan(Event * evt, action_fun bound_fun) : event(evt), fun(bound_fun) { };
    bool match_event(Event * evt);
    void execute(Context & ctx);
    void bind(Event * evt, Context & ctx);
    void unbind();
    friend ostream& operator<<(ostream & out, Plan & p);
};


class Event {
    Belief * bel;
    event_type type;
public:
    Event(Belief * b, event_type t);
    Event(Event & evt) { bel = evt.bel; type = evt.type; };
    Plan * operator>>(action_fun fun);
    Belief * get_belief() { return bel; };
    bool operator==(Event & evt);
    friend ostream& operator<<(ostream & out, Event & ev);
};


class Belief {
public:
    Belief(term_vector & t) { _terms = t; };
    Belief(const Belief & bel) { name = bel.name; _terms = bel._terms; };

    virtual void set_name(string n) { name = n; };
    string & get_name() { return name; };

    term_vector & get_terms() { return _terms; };

    bool match_ground(Belief & b);
    bool match_and_bind(Context & c, Belief & b);
    bool match_free(Belief & b);
    void unbind();

    Event operator+() { return Event(new Belief(*this), add);};
    bool operator==(Belief & bel);

    friend ostream& operator<<(ostream & out, Belief & b);

private:
    term_vector _terms;
    string name;
};

ostream& operator<<(ostream & out, Belief & b);
ostream& operator<<(ostream & out, Event & ev);
ostream& operator<<(ostream & out, Plan & p);



class KnowledgeBase {
    map<string, vector<Belief *>> kb;
 public:
    KnowledgeBase() { };
    int assert_belief(Belief * bel);
    int retract_belief(Belief * bel);
    vector<Belief *> * get_named_beliefs(string & name);
    void show();
};


#define bel_preamble(name) \
 _##name(term_vector & t) : Belief(t) { create(); }


#define belief(name) \
    class _##name : public Belief { \
public:                          \
 bel_preamble(name) \
     virtual void create() { set_name(#name); };  \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }


#define var(V) term V(true, #V);

#define get(typ, nam, c) typ nam=c[#nam]

class Engine {
    KnowledgeBase kb;
    static Engine * current;
    vector<Plan*> plans;
    TSQueue<Event*> event_queue;
    thread * main_thread;
    void run();
 public:
    Engine() { current = this; };
    Engine & operator+(Belief b);
    void add_plan(Plan * p) { plans.push_back(p); };
    void show_kb() { kb.show(); };
    void show_plans();
    void start();
    static Engine & get_current() { return *current; };

    friend void thread_start(Engine * e);

};



#endif
