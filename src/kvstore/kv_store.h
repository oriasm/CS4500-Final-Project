#include <stdlib.h>
#include "../utilities/object.h"
#include "../utilities/map.h"
#include "../utilities/serial.h"
#include "value.h"
#include "../dataframe/dataframe.h"

class KVStore : public Object
{
public:
    Map *map;

    KVStore()
    {
        map = new Map();
    }

    ~KVStore()
    {
        delete map;
    }

    void put(Key &key, Value *v)
    {
        char *serialized = v->serialize();
        String *str = new String(serialized);
        map->put(&key, str);
    }

    Value *get(Key &key)
    {
        String *serialized = map->get(&key);
        char *serialized_char = serialized->c_str();
        Object *obj = Deserialize::deserialize(serialized_char);
        return new Value(obj);
    }

    Value *getAndWait(Key &key)
    {
        while(true) {
            if(map->containsKey(&key)) {
                return get(key);
            }
        }
    }
};