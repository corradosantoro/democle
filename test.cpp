/*
 * test.cpp
 */


#include <iostream>

#include "democle.h"

using namespace std;

belief(coin);
belief(amount);
belief(amount2);
var(X);
var(Y);
goal(coin50);

int main(int argc , char **argv)
{
    Engine e("main");

    coin50() << (coin(50) & amount(50));

    +coin(X) / (amount(X) & amount2(X)) >> [](Context & c)
    {
        ctx_get(int, X, c);
        cout << "hello1 coin + amount " << X << endl;
        cut();
        cout << "hello2 coin + amount " << X << endl;
        //c + coin(50, "one");
    };

    +coin(X) >> [](Context & c) {
        ctx_get(int, X, c);
        cout << "Other plan " << X << endl;
    };

    +coin(X, Y) >> [](Context & c)
    {
        ctx_get(int, X, c);
        ctx_get(string, Y, c);
        cout << "hello[2] " << X << "," << Y << endl;
    };


    e.show_plans();
    e.start();

    e + amount(30);
    e + amount(40);
    e + amount2(30);

    //e + coin(10, 20, 30, "X");
    //e + coin(X); // not valid, cannot assert a belief with an unbound var
    e + coin(30);
    //e + coin(50, "one");
    e + coin(40);
    e + coin();

    sleep(1);

    //e.show_kb();
    // kb.assert_belief(c1);
    // kb.assert_belief(c2);
    // kb.assert_belief(c3);
    // kb.assert_belief(c4);
    // kb.show();

    return 0;
}
