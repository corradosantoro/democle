/*
 * sensor.cpp
 */

#include "sensor.h"
#include <iostream>

void sensor_thread_proc(Sensor * s)
{
    s->run();
}

void Sensor::start()
{
    sensor_thread = new thread(sensor_thread_proc, this);
    sensor_thread->detach();
}


void Sensor::run()
{
    while (true) {
        sense();
    }
}
