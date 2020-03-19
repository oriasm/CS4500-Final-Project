#include <stdlib.h>

class Application
{
public:
    size_t node;
    Application(size_t index)
    {
        node = index;
    }

    size_t this_node()
    {
        return node;
    }

    virtual void run()
    {
    }
};