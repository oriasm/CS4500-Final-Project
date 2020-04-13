#include <stdlib.h>
#include "../kvstore/key.h"

/**
 * Author: Maria Orias
 * Base class for applications
 */ 
class Application
{
public:
    KDStore *kv;
    size_t node;
    /**
     * Default constructor for applications take in the instance or ndoe index
     */
    Application(size_t index)
    {
        node = index;
    }

    // Returns the index of this application's node
    size_t this_node()
    {
        return node;
    }

    // Runs the main functionality of this application
    virtual void run()
    {
    }
};