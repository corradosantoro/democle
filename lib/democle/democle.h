/*
 * democle.h
 */

#ifndef __DEMOCLE_H
#define __DEMOCLE_H

#include <string>

#include "democle_types.h"
#include "goal.h"
#include "plan.h"
#include "engine.h"
#include "agent.h"
#include "sensor.h"
#include "sensor_lib.h"
#include "protocol.h"
#include "packet.h"
#include "errors.h"

#include <map>

using namespace std;

class DEMOCLE {
    static DEMOCLE * _instance;
    std::map<string, Agent *> registry;
 public:
    DEMOCLE();
    static DEMOCLE * instance() {
        if (_instance == nullptr) {
            _instance = new DEMOCLE();
        }
        return _instance;
    };
    void register_agent(Agent * a);
    Agent * get_agent(string name);
    void _register_tcp_protocol();

    static void register_protocol(string protocol_name) {
        if (protocol_name == "tcp")
            instance()->_register_tcp_protocol();
    };
};


#endif
