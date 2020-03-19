#include <stdlib.h>
#include "../utilities/object.h"
#include "../kvstore/value.h"
#include "../kvstore/kv_store.h"
#include "../dataframe/dataframe.h"

// A wrapper class for a kv_store that returns and accepts references to a Dataframe as the Values
class KDStore : public Object
{
public:
    KVStore *store;

    //default constructor
    KDStore()
    {
        store = new KVStore();
    }

    // default destructor
    ~KDStore()
    {
        delete store;
    }

    // stores the given DF with the given key
    void put(Key &key, DataFrame *v)
    {
        Value *temp = new Value(v);
        store->put(key, temp);
        delete temp;
    }

    // gets teh DF with the given key in the kvstore
    DataFrame *get(Key &key)
    {
        Value *temp = store->get(key);
        Object *obj = temp->deserialize();
        delete temp;
        return dynamic_cast<DataFrame *>(obj);
    }

    // waits for the given key to have a dataframe in the kvstore and returns it
    DataFrame *getAndWait(Key &key)
    {
        Value *temp = store->getAndWait(key);
        Object *obj = temp->deserialize();
        delete temp;
        return dynamic_cast<DataFrame *>(obj);
    }
};