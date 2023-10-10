/*
 * sensor_lib.cpp
 */

#include "democle_types.h"
#include "sensor_lib.h"
#include <unistd.h>
#include <iostream>
#include <sys/time.h>


reactor(tick);

void Timer::sense()
{
    usleep(mills * 1000);
    (*agent) + tick(name);
}


#ifdef HAS_EMBEDDED

DigitalInputEventHandler * DigitalInputEventHandler::_instance = nullptr;
bool DigitalInputEventHandler::started = false;

void digital_input_thread_start(void * x)
{
    DigitalInputEventHandler::instance()->run();
}

void DigitalInputEventHandler::send_event(int pin)
{
    //std::unique_lock<std::mutex> lock(m_mutex);
    event_pin = pin;
    //m_cond.notify_one();
}

void DigitalInputEventHandler::add(int pin, AtomicFormula * f, Agent * a)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    event[pin] = false;
    belief_map[pin] = f;
    agent_map[pin] = a;

    if (!started)
        start();
}

void DigitalInputEventHandler::start()
{
    event_thread = new std::thread(digital_input_thread_start, this);
    event_thread->detach();
    started = true;
}

void DigitalInputEventHandler::run()
{
    while (true) {
        usleep(10000l);
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            //m_cond.wait(lock, [this]() { return event_pin >= 0; });

            if (event_pin >= 0) {
                int pin = event_pin;
                Agent * a = agent_map[pin];
                AtomicFormula * b = belief_map[pin];
                (*a) + (*b);
                event[pin] = false;
                event_pin = -1;
            }
        }
    }
}

#endif

