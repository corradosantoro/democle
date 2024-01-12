/*
 * intention.h
 */

#ifndef __INTENTION_H
#define __INTENTION_H

#include "plan.h"

class Intention {
    Plan * pln;
    Context * ctx;
 public:
    Intention(Plan * p, Context * c) : pln(p), ctx(c) { };
    Context * context() { return ctx; };
    Plan * plan() { return pln; };
};

#endif
