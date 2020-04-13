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
#include <thread>
#include "../utilities/object.h"
#include "peer_array.h"

// Size of REG msg
const size_t REG_SIZE = 32;
// Size of MSG msg and command
const size_t MSG_SIZE = 4096;
// socket timeout
const nfds_t TIMEOUT = 100;
// # of file descriptors
const nfds_t NUM_FD = 50;

/**
 * Author: Maria Orias
 *  Creates a generic Socket Object capable of binding to an ip and port
 */
class Socket : public Object
{
public:
    // how many nodes have registered
    int peers_registered;
    // this socket's fd
    int my_fd;
    // my address
    struct sockaddr_in my_addr;
    // array of peers
    PeerArray *peers;
    // my info
    int my_port;
    char *my_ip;
    int my_id;
    struct pollfd *fds;

    // Constructor to create socket bound to given ip and port
    Socket(char *ip, int port)
    {
        // Iniatialize vars
        peers = new PeerArray();
        peers_registered = 0;

        // save my contact info
        my_port = port;
        my_ip = new char[INET_ADDRSTRLEN];
        memcpy(my_ip, ip, strlen(ip));
        my_id = -1;

        // create socket
        assert((my_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) != 0);

        // assign IP & port
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(port);
        // Check given ip address
        if (inet_pton(AF_INET, ip, &my_addr.sin_addr.s_addr) <= 0)
        {
            printf("Invalid address\n");
            exit(-1);
        }

        // Bind to ip and port
        assert(bind(my_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) >= 0);
        assert(listen(my_fd, 3) >= 0);

        //Add stdin and my fd to array of fd
        fds = new pollfd[NUM_FD];
        fds[0].fd = STDIN_FILENO;
        fds[0].events = POLLIN;
        fds[1].fd = my_fd;
        fds[1].events = POLLIN;
    }

    // default destructor
    virtual ~Socket()
    {
        // close sockets
        for (int i = 0; i < peers_registered; i++)
        {
            close(peers->get(i)->socket);
        }
        close(my_fd);

        delete peers;
    }

    // Called in subclasses to begin executing a specific socket's function
    virtual void start() {}

    /** PRIVATE FUNCTIONS **/

    // returns the ip address of the given peer
    char *get_client_info_(struct sockaddr_in client_addr)
    {
        // get peer ip
        char *ip_addr = new char[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_addr, INET_ADDRSTRLEN);

        return ip_addr;
    }

    // Used to contact a peer at the given IP and port for the first time
    // Sends the given msg
    // Returns the file descriptor associated with that peer's socket
    // Port is in host byte order
    virtual int contact_peer_(Peer *peer, char *msg)
    {
        int peer_sock;
        struct sockaddr_in peer_addr;
        // create tcp socket
        assert((peer_sock = socket(AF_INET, SOCK_STREAM, 0)) >= 0);
        // set server addr
        peer_addr.sin_family = AF_INET;
        peer_addr.sin_port = htons(peer->port);
        // check ip address
        if (inet_pton(AF_INET, peer->ip, &peer_addr.sin_addr.s_addr) <= 0)
        {
            printf("\nInvalid address/ Address not supported: %s\n", peer->ip);
            exit(-1);
        }

        // Connect to peer
        connect(peer_sock, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
        // send msg
        send(peer_sock, msg, strlen(msg), 0);

        return peer_sock;
    }

    // Polls stdin (for commands to send msgs) and
    // my file descriptor (for incoming msgs)
    virtual void poll_for_msgs()
    {
        int ret;
        while (true)
        {
            // when ret > 0, we have a msg on one of the file descriptors
            ret = poll(fds, peers_registered + 2, TIMEOUT);

            if (ret == -1)
            {
                printf("Polling error");
                exit(1);
            }

            for (int index = 0; index < peers_registered + 2; index++)
            {
                struct pollfd curr_fd = fds[index];
                if (curr_fd.revents & POLLIN)
                {
                    if (curr_fd.fd == my_fd)
                    {
                        // accept msg on my_fd
                        struct sockaddr_in client_addr;
                        socklen_t len = sizeof(client_addr);
                        int conn_fd = accept(curr_fd.fd, (struct sockaddr *)&client_addr, &len);
                        // read and parse msg
                        parse_msg_(conn_fd, client_addr);
                    }
                    else
                    {

                        parse_msg_(curr_fd.fd);
                    }
                }
            }
        }
    }

    // Sub classes override to read and parse msgs on the given file descriptor and sender's info
    virtual void parse_msg_(int fd, sockaddr_in &client_addr) { printf("base class Socket cannot parse a message"); }

    // sub classes override to read and parse msgs on stdin
    virtual void parse_msg_(int fd) { printf("base class Socket cannot parse a message"); }
};

// A ServerSocket object that functions as a server, receving msgs from clients
class ServerSocket : public Socket
{
public:
    // Calls Socket constructor to create a socket bound to given ip and port
    // Begins accepting client connections
    ServerSocket(char *ip, int port) : Socket(ip, port) {}

    // Server begins waiting for clients
    virtual void start()
    {
        // std::thread *polling = new std::thread(&ServerSocket::poll_for_msgs, this);
        poll_for_msgs();
    }

    /** PRIVATE FUNCTIONS **/

    // Parses msg on stdin
    void parse_msg_(int fd)
    {
        char incoming[MSG_SIZE] = {0};
        read(fd, incoming, MSG_SIZE);
        size_t command_length = strlen(incoming);
        assert(command_length < MSG_SIZE);
        incoming[command_length] = '\0';

        // get msg type
        char *type = strtok(incoming, "\n");
        if (!type)
            return;

        if (strcmp(type, "KILL") == 0)
        {
            for (int i = 0; i < peers_registered; i++)
            {
                Peer *peer = peers->get(i);
                contact_peer_(peer, type);
            }

            exit(0);
        }
    }

    // Parses msg on the given socket file descriptor
    void parse_msg_(int fd, sockaddr_in &client_addr)
    {
        // if false, msg format was incorrect
        if (!read_from_client_(fd, client_addr))
            return;

        //send new client's info to all clients
        send_client_update_();
    }

    // Returns a string with each clients info on a new line
    char *generate_client_list_()
    {
        StrBuff *buffer = new StrBuff();
        buffer->c("DIR ");
        for (int i = 0; i < peers_registered; i++)
        {
            // get client info
            Peer *peer = peers->get(i);
            int port = peer->port;
            char *ip = peer->ip;
            int id = peer->id;

            // put each port and ip on new line
            char *sm_buffer = new char[32];
            sprintf(sm_buffer, "%d %s %d ", id, ip, port);
            // add to buffer
            buffer->c(sm_buffer);
        }

        // finalize buffer
        String *str = buffer->get();
        delete buffer;
        // get char*
        char *result = str->steal();
        delete str;

        return result;
    }

    // Sends all clients an update of the latest client who registered
    void send_client_update_()
    {
        // Last client added is new
        int target_node = peers_registered - 1;
        // get new client info
        Peer *latest = peers->get(target_node);

        // construct message: [client IP] [client PORT] for each peer info
        char *msg = generate_client_list_();

        // Send new client first message
        contact_peer_(latest, msg);

        // send clients an update
        for (int i = 0; i < target_node; i++)
        {
            Peer *curr = peers->get(i);
            // contact_peer_(curr, msg);
            send(curr->socket, msg, strlen(msg), 0);
        }

        delete msg;
    }

    // Reads from the given socket fd, verifies its REG [PORT] msg, returns PORT
    bool read_from_client_(int socket_fd, sockaddr_in &client_addr)
    {
        char buffer[REG_SIZE];

        // Read from socket
        recv(socket_fd, buffer, sizeof(buffer), 0);
        // Verify this is a REG msg
        if (buffer[0] != 'R' || buffer[1] != 'E' || buffer[2] != 'G')
            return false;
        // generate a new id for each new node
        int id = rand() % 1000;
        // Get port and id from msg
        int port;
        sscanf(buffer, "REG %d", &port);
        // Get IP address from socket info
        char *client_ip = get_client_info_(client_addr);

        // Write to stdout
        printf("<---From client %d at <%s:%d>: REG\n", id, client_ip, port);

        // save client info
        Peer *peer = new Peer(port, socket_fd, client_ip, id);
        peers->push(peer);
        fds[peers_registered + 2].fd = socket_fd;
        fds[peers_registered + 2].events = POLLIN;
        peers_registered += 1;

        return true;
    }
};

class ClientSocket : public Socket
{
public:
    char *server_ip;
    int server_port;
    int server_fd;
    std::thread *polling;

    // Constructor to create network object bound to given ip
    ClientSocket(char *ip, int port) : Socket(ip, port) {}

    // Registers this socket with the server
    // Send sign up to server at the given port and IP
    virtual void start(char *ip, int port)
    {
        // Save server info, external variable
        server_ip = ip;
        server_port = port;

        // Construct register msg: 'REG [port]'
        unsigned int my_port = ntohs(my_addr.sin_port);
        char *msg = new char[REG_SIZE];
        sprintf(msg, "REG %u\n", my_port);
        // make server a 'peer'
        Peer *server = new Peer(server_port, -1, server_ip, -1);
        // send to server
        server_fd = contact_peer_(server, msg);

        // add server to list of fds to poll
        fds[2].fd = server_fd;
        fds[2].events = POLLIN;
        // wait for msgs
        polling = new std::thread(&ServerSocket::poll_for_msgs, this);
        // poll_for_msgs();
    }

    // Sends a message to a client
    void send_msg_to_client(char *msg, int peer_id)
    {
        char *formatted = new char[MSG_SIZE];
        sprintf(formatted, "MSG %d %s\n", my_id, msg);

        for (int i = 0; i < peers_registered; i++)
        {
            if (peers->get(i)->id == peer_id)
            {
                Peer *curr = peers->get(i);
                int possible_sock = curr->socket;
                // Check if we have a socket open already
                if (possible_sock != -1)
                {
                    send(possible_sock, formatted, strlen(formatted), 0);
                }
                else
                {
                    // open new connection
                    int peer_sock = contact_peer_(curr, formatted);
                    // curr->socket = peer_sock;
                    fds[i + 3].fd = peer_sock;
                    fds[i + 3].events = POLLIN;
                }
            }
        }
    }

    /** PRIVATE METHODS **/

    void poll_for_msgs()
    {
        int ret;
        while (true)
        {
            // when ret > 0, we have a msg on one of the file descriptors
            ret = poll(fds, peers_registered + 3, TIMEOUT);

            if (ret == -1)
            {
                printf("Polling error");
                exit(1);
            }

            for (int index = 0; index < peers_registered + 3; index++)
            {
                struct pollfd curr_fd = fds[index];
                if (curr_fd.revents & POLLIN && curr_fd.fd != -1)
                {
                    if (curr_fd.fd == my_fd)
                    {
                        // accept msg on my_fd
                        struct sockaddr_in client_addr;
                        socklen_t len = sizeof(client_addr);
                        int conn_fd = accept(curr_fd.fd, (struct sockaddr *)&client_addr, &len);
                        // read and parse msg
                        parse_msg_(conn_fd, client_addr);
                    }
                    else
                    {
                        parse_msg_(curr_fd.fd);
                    }
                }
            }
        }
    }

    // Used by base class to parse msgs from socket
    void parse_msg_(int fd, sockaddr_in &client_addr)
    {
        // ClientSocket handles all messages in parse_msg_(int fd)
        parse_msg_(fd);
    }

    // reads message on the given fd, parses based on type
    void parse_msg_(int fd)
    {
        char incoming[MSG_SIZE] = {0};
        read(fd, incoming, MSG_SIZE);
        size_t command_length = strlen(incoming);
        assert(command_length < MSG_SIZE);
        incoming[command_length] = '\0';

        // incoming msg is a KILL command
        if (strcmp(incoming, "KILL") == 0)
        {
            polling->join();
            exit(0);
        }

        // get msg type
        char *type = strtok(incoming, " \0");
        if (!type)
            return;

        // incoming msg is a command from stdin
        if (strcmp(type, "MSG") == 0 && fd == STDIN_FILENO)
        {
            // get dest node
            char *id_ptr = strtok(NULL, " ");
            // get msg to be sent
            char *msg = strtok(NULL, "\r");
            if (msg)
                handle_msg_command(id_ptr, msg);
        }
        // incoming msg is a msg from a client
        else if (strcmp(type, "MSG") == 0)
        {
            // reading sender
            char *peer = strtok(NULL, " ");
            // get actual msg
            char *actual_msg = strtok(NULL, "\r");
            handle_incoming_msg(fd, peer, actual_msg);
        }
        // incoming msg is a directory message from server
        else if (strcmp(type, "DIR") == 0)
        {
            // read first client id
            char *id_ptr = strtok(NULL, " ");

            while (id_ptr != NULL)
            {
                char *ip = strtok(NULL, " ");
                char *port_ptr = strtok(NULL, " ");

                // msg has port and ip
                if (ip && port_ptr)
                {
                    handle_dir_entry(id_ptr, ip, port_ptr);
                    // get next line
                    id_ptr = strtok(NULL, " ");
                }
            }
        }
    }

    // parses given peer info and saves it
    void handle_dir_entry(char *id_info, char *ip, char *port_info)
    {
        int port;
        int id;

        // get integers for port and id
        sscanf(id_info, "%d", &id);
        sscanf(port_info, "%d", &port);

        // save my id
        if (port == my_port && strcmp(ip, my_ip) == 0)
        {
            if (my_id == -1)
            {
                printf("<---My id: %d\n", id);
            }
            my_id = id;
        }
        // save other node's info
        else
        {
            save_client_info(id, ip, port);
            printf("<---DIR %d %s %d\n", id, ip, port);
        }
    }

    // parses char arrays from stdin to contact a client
    void handle_msg_command(char *peer_dest, char *payload)
    {
        // get destination peer
        int dest;
        sscanf(peer_dest, "%d", &dest);

        size_t msg_len = strlen(payload);
        // line read from stdin is not null terminated
        payload[msg_len] = '\0';

        send_msg_to_client(payload, dest);
    }

    // parses char arrays from socket to print incoming msg to stdout
    void handle_incoming_msg(int fd, char *peer_src, char *payload)
    {
        int id;
        sscanf(peer_src, "%d", &id);
        // get clients addr
        int index = find_client(id);
        int port;
        char *ip;
        // msg is from unregistered client
        if (index == -1)
        {
            port = 0;
            ip = new char();
        }
        // find clients information
        else
        {
            Peer *peer = peers->get(index);
            port = peer->port;
            ip = peer->ip;
            peer->socket = fd;
            fds[index + 3].fd = fd;
            fds[index + 3].events = POLLIN;
        }
        printf("<---From client %d at <%s:%d>: %s\n", id, ip, port, payload);
    }

    // Returns the index in the list of peer_ids that matches the
    // given peer id, -1 if not found
    int find_client(int id)
    {
        for (int i = 0; i < peers_registered; i++)
        {
            Peer *peer = peers->get(i);
            int curr_id = peer->id;
            if (curr_id == id)
            {
                return i;
            }
        }
        return -1;
    }

    // Saves each new client's info
    void save_client_info(int id, char *ip, int port)
    {
        for (int i = 0; i < peers_registered; i++)
        {
            int curr_id = peers->get(i)->id;
            if (curr_id == id)
            {
                // we already have this clients info
                /** TODO: update? existing info */
                return;
            }
        }

        // save info
        Peer *peer = new Peer(port, -1, ip, id);
        peers->push(peer);
        peers_registered += 1;
    }
};
