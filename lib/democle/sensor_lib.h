/*
 * sensor_lib.h
 */

#ifndef __SENSOR_LIB_H
#define __SENSOR_LIB_H

#include <map>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <utility>
#include "sensor.h"

class Timer : public Sensor {
    int mills;
    std::string name;
 public:
    Timer(int m, std::string _n) : Sensor(),mills(m),name(_n) { };
    virtual void sense();
};


#ifdef HAS_EMBEDDED

#include <Arduino.h>

class DigitalInputEventHandler {
    int event_pin;
    std::map<int, AtomicFormula *> belief_map;
    std::map<int, Agent *> agent_map;
    std::map<int, bool> event;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    TaskHandle_t event_task;
    static DigitalInputEventHandler * _instance;
    static bool started;
public:
    DigitalInputEventHandler() : event_pin(-1) { };
    static DigitalInputEventHandler * instance() {
        if (_instance == nullptr)
            _instance = new DigitalInputEventHandler();
        return _instance;
    };
    void start();
    void add(int pin, AtomicFormula * f, Agent * a);
    void send_event(int pin);
    void run();
};


#define map_input_pin(pin, edge, b)                                     \
    {                                                                   \
        AtomicFormula bx = b();                                         \
        AtomicFormula * belief_##pin = new AtomicFormula(bx);           \
        DigitalInputEventHandler::instance()->add(pin, belief_##pin, this); \
        setup_digital_##pin(edge);                                      \
    }




#define use_input_pin(pin)                                              \
    void ISR_digital_##pin()                                            \
    {                                                                   \
        DigitalInputEventHandler::instance()->send_event(pin);          \
    }                                                                   \
                                                                        \
    void setup_digital_##pin(int edge)                                  \
    {                                                                   \
        pinMode(pin, INPUT);                                            \
        attachInterrupt(digitalPinToInterrupt(pin), ISR_digital_##pin, edge); \
    }

#endif

#endif

