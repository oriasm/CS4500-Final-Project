// language: C++
#pragma once
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <cstring>
#include <cmath>
#include <cassert>
#include <poll.h>
#include "../utilities/object.h"
#include "../utilities/array.h"

/**
 * Author: Maria Orias
 * Represents a peer node in the network and holds identifying information
 */
class Peer : public Object
{
public:
    // registered peers' port number
    // stored in host byte order
    int port;
    // registered peers' opened socket
    int socket;
    // registered peers' ip address
    char *ip;
    // registered peers' node id
    int id;

    Peer(int p, int s, char *ip_addr, int my_id)
    {
        port = p;
        socket = s;
        ip = new char[INET_ADDRSTRLEN];
        memcpy(ip, ip_addr, INET_ADDRSTRLEN);
        id = my_id;
    }

    ~Peer()
    {
        delete ip;
    }
};