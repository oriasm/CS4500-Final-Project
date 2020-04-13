#include <cassert>
#include <stdlib.h>
#include "application.h"
#include "../kvstore/key.h"
#include "../dataframe/kd_store.h"
#include "../dataframe/dataframe.h"

/**
 *  Author: Maria Orias
 *  A trivial class that represents an application class that adds some floats to a DF 
 *  to retreive/verify them.
 */
class Trivial : public Application
{
public:
    /** Default constructor takes in this application's instance or node index 
     */ 
    Trivial(size_t idx) : Application(idx)
    {
        kv = new KDStore();
    }

    /**
     * Creates a float array, creates a DF from those values and stores it, verifies its correct
     */
    void run_()
    {
        size_t SZ = 1000 * 1000;
        float *vals = new float[SZ];
        float sum = 0;
        for (size_t i = 0; i < SZ; ++i)
            sum += vals[i] = i;
        Key *key = new Key("triv", 0);
        DataFrame *df = DataFrame::fromArray(key, kv, SZ, vals);
        assert(df->get_float(0, 1) == 1);
        DataFrame *df2 = kv->get(*key);
        for (size_t i = 0; i < SZ; ++i)
            sum -= df2->get_float(0, i);
        assert(sum == 0);
        delete df;
        delete df2;
    }
};