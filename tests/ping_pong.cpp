/*
 * ping_pong.cpp
 */


#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#include "democle.h"
#include "predicates.h"

using namespace std;

reactor(ping);
reactor(pong);
procedure(go);

class Receiver : public Agent {
public:
    Receiver() : Agent("receiver") {};
    void run() {

        accepts(ping/0);

        +ping() >> [] (Context & c)
        {
            cout << "Message received from " << c.sender() << endl;
            c + (c.sender(), pong());

        };

    };
};

class Sender : public Agent {
public:
    Sender() : Agent("sender") {};
    void run() {

        accepts(pong/0);

        go() >> [](Context & c)
        {
            //c.show_kb();
            cout << "Sending Message" << endl;
            c + ("tcp://localhost:4321/receiver", ping());
        };

        +pong() >> [] (Context & c)
        {
            cout << "Message received from " << c.sender() << endl;
            sleep(1);
            c << go();
        };

    };
};

int main(int argc , char **argv)
{

    if (argc == 1) {
        fprintf(stderr, "usage: %s --sender|--receiver\n", argv[0]);
    }

    if (!strcmp(argv[1],"--sender")) {

        DEMOCLE::register_protocol("tcp", "localhost", 4322);

        Sender s_agent;

        s_agent.start();

        s_agent << go();

    }
    else {

        DEMOCLE::register_protocol("tcp", "localhost", 4321);

        Receiver r_agent;
        r_agent.start();

        //r_agent << go();
    }

    sleep(60);

    return 0;
}
