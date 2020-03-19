#include <stdlib.h>
#include "../utilities/object.h"

// Holds a pointer to an object that presents a value in a KV store
class Value : public Object
{
public:
    Object *my_value;
    char *serialized;

    // Default constructor takes an Object pointer, that is unmodified by this class
    Value(Object *obj)
    {
        my_value = obj;
    }

    // Frees the serialized object, not the pointer to object
    ~Value() {
        delete serialized;
    }

    // serializes the object held in this Value class and returns it
    char *serialize()
    {
        if (!serialized)
        {
            serialized = my_value->serialize();
        }
        return serialized;
    }

    // returns a pointer to the object held in this Value class
    Object *deserialize()
    {
        return my_value;
    }
};