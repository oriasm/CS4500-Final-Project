#include <stdlib.h>
#include "trivial.h"


int main(int argc, char **argv)
{
    // Run a single instance of the Trivial application
    Trivial *t = new Trivial(0);
    t->run_();
    return 0;
}