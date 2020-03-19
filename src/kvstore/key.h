#include <stdlib.h>
#include "../utilities/object.h"

// Represents a Key object that has name and the index of the node that holds its information
class Key : public Object {
    char *name;
    size_t home_node;
public:
    Key(char *n, size_t index) {
        size_t len = strlen(n);
        name = new char[len];
        memcpy(name, n, len);
    }

    ~Key() {
        delete name;
    }
};