#include <stdlib.h>
#include "trivial.h"

int main(int argc, char **argv)
{
    Trivial *t = new Trivial(0);
    t->run_();
    return 0;
}