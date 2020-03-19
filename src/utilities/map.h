//lang::CwC
#pragma once

// #include "object.h"
#include "string.h"
#include "linked-list.h"

// An Object to hold a key and a value object
// This List does NOT own its elements, they must be freed independently
class KeyValuePair : public Object
{
public:
    size_t key;
    String *value;

    // Default constructor
    KeyValuePair(Object *k, String *v)
    {
        key = k->hash_me();
        value = v;
    }

    ~KeyValuePair() {}

    size_t hash()
    {
        return (key + value->hash_me()) * 31;
    }

    bool equals(Object *other)
    {
        KeyValuePair *pair = dynamic_cast<KeyValuePair *>(other);

        if (pair)
        {
            if (key == pair->key)
                return value->equals(reinterpret_cast <Object*>(pair->value));
        }

        return false;
    }

    void update(String *v)
    {
        value = v;
    }
};

// A map class that supports String:String mappings
// This class does not own any of the objects that receives
// User must free each object
class Map : public Object
{
    LinkedList *list;

public:
    // Constructor
    Map()
    {
        list = new LinkedList();
    }

    // Destructor
    ~Map()
    {
        delete list;
    }

    // Add an Object to the map with a String key
    void put(Object *key, String *val)
    {
        String *obj = get(key);

        if (obj)
        {
            KeyValuePair *pair = new KeyValuePair(key, obj);
            size_t index = list->index_of(pair);
            if (index > list->size())
            {
                KeyValuePair *pair = new KeyValuePair(key, val);
                list->push_back(pair);
            }
            else
            {
                pair->update(val);
                Object* old = list->set(index, pair);
                delete old;
            }
        }
        else
        {
            KeyValuePair *pair = new KeyValuePair(key, val);
            list->push_back(pair);
        }
    }

    // Remove the value tied to the key from the map. Removes the key as well.
    void remove(Object *key)
    {
        size_t size = list->size();

        for (size_t i = 0; i < size; i++)
        {
            KeyValuePair *temp = static_cast<KeyValuePair *>(list->get(i));
            if (temp->key == key->hash_me())
            {
                Object *removed = list->remove(i);
                delete removed;
                return;
            }
        }
    }

    // Gets the value associated with the key
    String *get(Object *key)
    {
        size_t size = list->size();

        for (size_t i = 0; i < size; i++)
        {
            KeyValuePair *temp = static_cast<KeyValuePair *>(list->get(i));
            if (temp->key == key->hash_me())
            {
                return temp->value;
            }
        }

        return NULL;
    }

    // Clears all the keys and values from the map
    void clear()
    {
        list->clear();
    }

    // Is the key in the map?
    bool containsKey(Object *key)
    {
        size_t size = list->size();

        for (size_t i = 0; i < size; i++)
        {
            KeyValuePair *temp = static_cast<KeyValuePair *>(list->get(i));
            if (temp->key == key->hash_me())
            {
                return true;
            }
        }
        return false;
    }

    // Get the number of keys in the map
    size_t size()
    {
        return list->size();
    }

    // Is this map eual to the Object?
    bool equals(Object *o)
    {
        Map *map = dynamic_cast<Map *>(o);

        if (map)
        {
            return list->hash() == map->list->hash();
        }

        return false;
    }

    // Get the hash of the map object
    size_t hash()
    {
        return list->hash();
    }
};
