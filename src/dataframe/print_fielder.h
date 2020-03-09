// Language: CwC
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include "fielder.h"

// Fielder class that prints each value to console on the same line
class PrintFielder : public Fielder
{
public:
    PrintFielder() {}

    virtual void start(size_t r) {}

    void accept(bool b)
    {
        printf("%d ", b);
    }

    void accept(float f)
    {
        printf("%f ", f);
    }

    void accept(int i)
    {
        printf("%d ", i);
    }

    void accept(String *s)
    {
        char *val = s->steal();
        printf("\"%s\" ", val);
    }

    void done() {
        printf("\n");
    }   
};