#pragma once
#include <stdlib.h>
#include "../utilities/object.h"

/** 
 * Author: Maria Orias
 * Represents a Key object that has name and the index of the node that holds its information
 */
class Key : public Object {
    // key name
    char *name;
    // index of node that holds this key's value
    size_t home_node;
public:
    // default constructor takes in the key name and the node that will hold its value
    Key(char *n, size_t index) {
        size_t len = strlen(n);
        name = new char[len];
        memcpy(name, n, len);
    }

    // default destructor
    ~Key() {
        delete name;
    }
};