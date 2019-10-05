#ifndef DYNAMO_NET_SERVER_H_
#define DYNAMO_NET_SERVER_H_

#include <SDL2/SDL_net.h>

#include <string>
#include <vector>
#include <utility>
#include <cstring>

#include "../error.h"

namespace Dynamo::Net {
    // Default protected network protocols
    typedef enum {
        NET_CONNECT = 0,
        NET_DISCONNECT = 1,
        NET_ALIVE = 3,
    } NET_PROTOCOL;

    class Server {
    protected:
        UDPsocket socket_;

        IPaddress host_;
        std::string hostname_;
        std::string ip_string_;

        // Keeps track of connected clients and response times
        std::vector<std::pair<IPaddress, int>> connected_;

        UDPpacket *recv_;
        size_t packet_size_;

    public:
        Server(int port, size_t packet_size_);
        ~Server();

        // Get host name (normally the name on the computer)
        std::string get_hostname();

        // Get server address as a domain string (i.e. XXX.XX.XX.X)
        std::string get_ip();

        // Update the server
        void tick();

        // Add an IP address to the collection of connections
        bool add_client(IPaddress address);

        // Remove a connected IP address
        bool remove_client(IPaddress address);

        // Reset the timer of a client
        bool reset_client(IPaddress address);

        // Send to a particular address, returns bytes sent
        int send(IPaddress *target, void *data, int len, int protocol);

        // Send to all connected clients
        void send_all(void *data, int len, int protocol);
        
        // Listen for client packets
        int listen();

        // What do to with packet received
        // Custom protocols must not override NET_PROTOCOL
        virtual void action(void *data, int len, int protocol) = 0;
    };
}

#endif