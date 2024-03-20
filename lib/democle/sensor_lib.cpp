/*
 * sensor_lib.cpp
 */

#ifdef HAS_EMBEDDED
#include <Arduino.h>
#endif
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
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t ulStatusRegister;

    xTaskNotifyIndexedFromISR( event_task,
                               0,
                               1 << pin,
                               eSetBits,
                               &xHigherPriorityTaskWoken );
    //event_pin = pin;
}

void DigitalInputEventHandler::add(int pin, AtomicFormula * f, Agent * a)
{
    event[pin] = false;
    belief_map[pin] = f;
    agent_map[pin] = a;

    if (!started)
        start();
}

void DigitalInputEventHandler::start()
{
    xTaskCreate((TaskFunction_t)digital_input_thread_start, "DigitalInputEventHandler",
                    4096, this, tskIDLE_PRIORITY, &event_task);
    started = true;
}

void DigitalInputEventHandler::run()
{
    uint32_t notifiedValue;
    while (true) {
        xTaskNotifyWaitIndexed( 0,         /* Wait for 0th notification. */
                                0x00,      /* Don't clear any notification bits on entry. */
                                ULONG_MAX, /* Reset the notification value to 0 on exit. */
                                &notifiedValue, /* Notified value pass out in
                                                     ulNotifiedValue. */
                                portMAX_DELAY );  /* Block indefinitely. */
        for (int event_pin = 0; notifiedValue != 0; event_pin++) {
            if ((notifiedValue & 1) != 0) {
                int pin = event_pin;
                Agent * a = agent_map[pin];
                AtomicFormula * b = belief_map[pin];
                //std::cout << "BEGIN Generating event" << std::endl;
                (*a) + (*b);
                //std::cout << "END Generating event" << std::endl;
                event[pin] = false;
            }
            notifiedValue >>= 1;
        }
    }
}

#endif
