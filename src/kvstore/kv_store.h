#include <stdlib.h>
#include "../utilities/object.h"
#include "../utilities/map.h"
#include "../utilities/serial.h"
#include "value.h"
#include "../dataframe/dataframe.h"

// Represents a Key Value store object that uses a Map
class KVStore : public Object
{
public:
    Map *map;

    /** default constructor */
    KVStore()
    {
        map = new Map();
    }

    ~KVStore()
    {
        delete map;
    }

    /*
     *  Stores the given value with the given key  
     */
    void put(Key &key, Value *v)
    {
        char *serialized = v->serialize();
        String *str = new String(serialized);
        map->put(&key, str);
    }

    /*
     *   Returns the value associated with the given key
     */
    Value *get(Key &key)
    {
        String *serialized = map->get(&key);
        char *serialized_char = serialized->c_str();
        Object *obj = Deserialize::deserialize(serialized_char);
        return new Value(obj);
    }

    /*
     * Waits for the given key to exist, and returns the value associated with it   
     */
    Value *getAndWait(Key &key)
    {
        while(true) {
            if(map->containsKey(&key)) {
                return get(key);
            }
        }
    }
};