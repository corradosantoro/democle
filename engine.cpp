/*
 * engine.cpp
 */

//
// DEMOCLE: DEclarative Multi-agent Open C-Language Engine
//

#include "errors.h"
#include "engine.h"
#include "intention.h"
#include "sensor.h"
#include "debug.h"

Engine * Engine::current = nullptr;

bool KnowledgeBase::assert_belief(AtomicFormula * bel)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            if (b->match_ground(*bel))
                return false;
        }
        v.push_back(bel);
        return true;
    }
    else {
        vector<AtomicFormula*> v;
        v.push_back(bel);
        kb[bel->get_name()] = v;
        return true;
    }
}

bool KnowledgeBase::retract_belief(AtomicFormula * bel)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            if (b->match_ground(*bel)) {
                v.erase(it);
                delete b;
                return true;
            }
        }
    }
    return false;
}

vector<AtomicFormula *> * KnowledgeBase::get_named_beliefs(string & name)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (kb.count(name))
        return & kb[name];
    else
        return NULL;

}

bool KnowledgeBase::get_matching_beliefs(AtomicFormula * bel, Context * starting_context, ContextPtrVector * ctx_array)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    bool ret_val = false;
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            Context * ctx = new Context(*starting_context);
            bel->bind(ctx);
            if (bel->match_and_bind(*b)) {
                DEBUG(cout << "KnowledgeBase::get_matching_beliefs TRUE" << endl;);
                ret_val = true;
                ctx_array->push_back(ctx);
            }
            else {
                DEBUG(cout << "KnowledgeBase::get_matching_beliefs FALSE" << endl;);
                delete ctx;
            }
            bel->unbind();
        }
    }
    return ret_val;
}

bool KnowledgeBase::verify_matching_beliefs(Context * ctx, AtomicFormula * bel)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            bel->bind(ctx);
            if (bel->match_and_bind(*b)) {
                bel->unbind();
                return true;
            }
            bel->unbind();
        }
    }
    return false;
}

void KnowledgeBase::show()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    for (const auto& _pair  : kb) {
        auto &bel_vector = _pair.second;
        for (auto b : bel_vector ) {
            cout << *b << endl;
        }
    }
}


Engine & Engine::operator+(AtomicFormula b)
{
    if (b.is_reactor()) {
        event_queue.push(new Event(new AtomicFormula(b), add));
        return *this;
    }

    if (kb.assert_belief(new AtomicFormula(b))) {
        event_queue.push(new Event(new AtomicFormula(b), add));
    }

    return *this;
}

Engine & Engine::operator-(AtomicFormula b)
{
    if (b.is_belief()) {
        if (kb.retract_belief(new AtomicFormula(b))) {
            event_queue.push(new Event(new AtomicFormula(b), del));
        }
    }

    return *this;
}

Engine & Engine::operator<<(AtomicFormula b)
{
    if (b.is_procedure()) {
        event_queue.push(new Event(new AtomicFormula(b), call));
        return *this;
    }

    return *this;
}

void Engine::show_plans()
{
    for (vector<Plan*>::iterator it = plans.begin(); it != plans.end(); it++) {
        cout << (**it) << endl;
    }
}

void thread_start(Engine * e)
{
    e->run();
}

void Engine::run()
{
    cout << "[" << name << "] Agent started" << endl;

    for (vector<Sensor *>::iterator it = agent->get_sensors().begin();
         it != agent->get_sensors().end();
         it++)
        {
            Sensor * s = (*it);
            s->start();
        }


    while (true) {

        Event * evt = event_queue.pop();

        DEBUG(cout << "-------------------------------------------------" << endl;)
        DEBUG(cout << "Event queued : " << (*evt) << endl;)

        vector<Intention *> intentions;

        for (vector<Plan*>::iterator it = plans.begin(); it != plans.end(); it++) {
            Plan * p = *it;
            p->unbind();
            DEBUG(cout << "   Testing " << (*p) << endl);
            Context * c = new Context();
            if (p->match_event(evt, c)) {
                Condition * cond = p->get_condition();
                if (cond != nullptr) {
                    ContextPtrVector * context_array = new ContextPtrVector;
                    DEBUG(cout << "-->Evaluating Condition" << endl;);
                    bool eval_result = cond->eval(this, c, context_array);
                    DEBUG(show_context_vector(context_array););
                    DEBUG(cout << "-->" << endl;);
                    if (!eval_result || context_array->size() == 0) {
                        DEBUG(cout << "COND FALSE" << endl;);
                        delete c;
                        delete context_array;
                        continue;
                    }
                    DEBUG(cout << "COND true" << endl;);
                    ContextPtrVector::iterator it = context_array->begin();
                    Context * c = (*it);
                    it++;
                    for (; it != context_array->end(); it++) {
                        Context * c_to_delete = (*it);
                        DEBUG(cout << "DELETING context " << c_to_delete << endl;);
                        delete c_to_delete;
                    }
                    delete context_array;
                    DEBUG(cout << "   Candidate Intention for the event " << (*p) << endl;);
                    DEBUG(cout << (*c) << endl;);
                    intentions.push_back(new Intention(p, c));
                }
                else {
                    DEBUG(cout << "   Candidate Intention for the event " << (*p) << endl;);
                    DEBUG(cout << (*c) << endl;);
                    intentions.push_back(new Intention(p, c));
                }
            }
        }

        for (vector<Intention *>::iterator it = intentions.begin(); it != intentions.end(); it++) {
            Intention * i = (*it);
            Plan * p = i->plan();
            Context * c = i->context();
            c->set_engine(this);
            c->set_sender(evt->get_belief()->get_sender());
            DEBUG(cout << "   Executing Intention " << (*p) << endl;);
            p->bind(c);
            bool success = true;
            try {
                p->execute(c);
            }
            catch (CutPlanException ex) {
                success = false;
            }
            p->unbind();
            if (success)
                break;

            //cout << "   Unbound plan "  << (*p) << endl;
        }

        for (vector<Intention *>::iterator it = intentions.begin(); it != intentions.end(); it++) {
            Intention * i = *it;
            Plan * p = i->plan();
            Context * c = i->context();
            p->unbind();
            delete c;
            delete i;
        }

        delete evt;
    }
}

void Engine::start()
{
    main_thread = new thread(thread_start, this);
    main_thread->detach();
}

