#ifndef DYNAMO_NET_CLIENT_H_
#define DYNAMO_NET_CLIENT_H_

#include <SDL2/SDL_net.h>

#include <chrono>

#include "node.h"

namespace Dynamo::Net {
    class Client : public Node {
        IPaddress server_;
        std::string server_name_;
        std::string server_ip_;

        long long int timestamp_;
        int timeout_;

    public:
        // Timeout in seconds
        Client(std::string ip, int port, int packet_size, int timeout_s);

        // Get the server ip address
        uint32_t get_server_ip();

        // Get the server ip address as a readable string (XXX.XX.XX.X)
        std::string get_server_ip_string();

        // Get the name of the server
        std::string get_server_name();

        // Send data to the server
        int send(void *data, int len, int protocol);

        // Update the client, returns whether a packet was received
        bool tick();
    };
}

#endif