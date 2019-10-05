#ifndef DYNAMO_NET_CLIENT_H_
#define DYNAMO_NET_CLIENT_H_

#include <SDL2/SDL_net.h>

#include <string>
#include <cstring>

#include "server.h"

namespace Dynamo::Net {
    class Client {
        UDPsocket socket_;

        IPaddress host_;
        std::string hostname_;
        std::string ip_string_;

        UDPpacket *recv_;
        size_t packet_size_;

        bool connected_;

    public:
        Client(std::string ip, int port, size_t packet_size);
        ~Client();

        // Get host name (normally the name on the computer)
        std::string get_hostname();

        // Get server address as a domain string (i.e. XXX.XX.XX.X)
        std::string get_ip();

        // Update the client
        void tick();

        // Send to the server
        int send(void *data, int len, int protocol);

        // Listen for server packets
        int listen();

        // What do to with packet received
        // Custom protocols must not override NET_PROTOCOL
        virtual void action(void *data, int len, int protocol) = 0;
    };
}

#endif