#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

#ifdef ESP32
#include <esp_heap_caps.h>

void show_free_memory()
{
    uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    uint32_t totalHeapBytes = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    // Print to serial
    Serial.printf("[Memory] %d of %d bytes free\n\r", freeHeapBytes, totalHeapBytes);
}
#endif

using namespace std;

#define FLASH_TEST

#ifdef CALC_MAX

belief(number);
procedure(calc_max);

class CalcMax : public Agent {
public:
    CalcMax() : Agent("calc_max") {};
    void run() {

        var(X);
        var(M);

        calc_max() / number(X)  >> [X](Context & c)
        {
            int x = c[X];
            cout << "Starting from " << x << endl;
            c << calc_max(X);
        };

        calc_max(M) / (number(X) & gt(X, M)) >> [X](Context & c)
        {
            int x = c[X];
            cout << "Continuing with " << x << endl;
            c << calc_max(x);
        };

        calc_max(M) >> [M](Context & c)
        {
            int m = c[M];
            cout << "Max is " << m << endl;
            show_free_memory();
        };


    };
};


CalcMax a;

void setup_max() {
    a.start();

    a.show_plans();

    srand(time(NULL));
    for (int i = 0; i < 100;i++) {
        a + number((int)(rand() * 3000.0 / RAND_MAX));
    }
    a.show_kb();
    show_free_memory();
    a << calc_max();

}

#endif

#ifdef SIEVE

belief(number);
reactor(test);
procedure(sieve);
procedure(show_primes);

class SieveAgent : public Agent {
public:
    SieveAgent() : Agent("sieve") {};
    void run() {

        var(X);
        var(Y);

        sieve() / (number(X) & number(Y) & neq(X,Y) & multiple(X, Y)) >> [X](Context & c)
        {
            int x = c[X];
            c - number(x);
            c << sieve();
        };

        sieve() >> [](Context & c)
        {
            //c.show_kb();
            c << show_primes();
        };

        show_primes() / number(X) >> [X](Context & c)
        {
            int x = c[X];
            cout << "Prime number " << x << endl;
            c - number(x);
            c << show_primes();
        };

        show_primes() >> [](Context & c)
        {
            cout << "end" << endl;
            c.show_kb();
            show_free_memory();
        };

    };
};


SieveAgent s_agent;

void setup_sieve() {
    s_agent.start();

    s_agent.show_plans();

    for (int i = 19; i >= 2;i--) {
        s_agent + number(i);
    }
    show_free_memory();
    s_agent << sieve();
}

#endif

#ifdef FLASH_TEST

reactor(tick);
belief(state1);
belief(state2);
belief(state3);

use_input_pin(22);

class FlashAgent : public Agent {
public:
    FlashAgent() : Agent("flash_agent") { };
    void run() {
        var(X);
        var(T);

        map_input_pin(22,FALLING,tick);

        //Timer * t1 = new Timer(500, "tim1");
        //Timer * t2 = new Timer(300, "tim2");
        //attach(t1);
        //attach(t2);

        pinMode(2, OUTPUT);
        pinMode(26, OUTPUT);
        pinMode(27, OUTPUT);

        +tick(string("tim1")) / state1(X)  >> [X](Context & c)
        {
            int x = c[X];
            cout << "Tick from timer 1:" << x << endl;
            digitalWrite(2, x);
            c - state1(x);
            x = !x;
            c + state1(x);
        };

        +tick(string("tim2")) / state2(X)  >> [X](Context & c)
        {
            int x = c[X];
            cout << "Tick from timer 2:" << x << endl;
            digitalWrite(26, x);
            c - state2(x);
            x = !x;
            c + state2(x);
        };

        +tick(string("tim3")) / state3(X)  >> [X,T](Context & c)
        {
            string t = c[T];
            int x = c[X];
            cout << "Tick from timer " << t << ":" << x << endl;
            digitalWrite(27, x);
            c - state3(x);
            x = !x;
            c + state3(x);
        };

        +tick() / state3(X) >> [X](Context & c)
        {
            int x = c[X];
            cout << "Pushbuttton " << ":" << x << endl;
            digitalWrite(27, x);
            //show_free_memory();
            c - state3(x);
            //show_free_memory();
            x = !x;
            c + state3(x);
            show_free_memory();
            //c.show_kb();
        };

    };
};

FlashAgent f;

void go()
{
    //f + tick(string("tim3"));
}
void setup_flash() {
    f.start();
    f + state1(0);
    f + state2(0);
    f + state3(0);
    //pinMode(22, INPUT);
    //attachInterrupt(digitalPinToInterrupt(22), go, FALLING);

}

#endif


void setup() {
    Serial.begin(115200);
#ifdef SIEVE
    setup_sieve();
#endif
#ifdef CALC_MAX
    setup_max();
#endif
#ifdef FLASH
    setup_flash();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
    sleep(10);
}

