#ifndef DYNAMO_NET_CLIENT_H_
#define DYNAMO_NET_CLIENT_H_

#include <SDL2/SDL_net.h>

#include "node.h"
#include "packet.h"

namespace Dynamo::Net {
    class Client : public Node {
        IPaddress server_;
        std::string server_ip_;

        int time_;
        int timestamp_;
        int timeout_;

    public:
        Client(std::string ip, int port, int packet_size, int timeout_s);

        // Get the server ip address
        uint32_t get_server_ip();

        // Get the server ip address as a readable string (XXX.XX.XX.X)
        std::string get_server_ip_string();

        // Send data to the server
        int send(void *data, int len, int protocol);

        // Test if this client is connected to the server
        bool is_connected();

        // Check if received new packet from server
        bool server_message();

        // Update the client
        bool update(int dt);
    };
}

#endif