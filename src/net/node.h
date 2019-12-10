#ifndef DYNAMO_NET_NODE_H_
#define DYNAMO_NET_NODE_H_

#include <SDL2/SDL_net.h>

#include <string>
#include <cstring>

#include "packet.h"
#include "../log/error.h"

namespace Dynamo::Net {    
    typedef enum {          // Default protocols
        NET_CONNECT = 0,    // Connection request
        NET_DISCONNECT = 1, // Disconnection request
        NET_ALIVE = 2,      // Status packet
    } NET_PROTOCOL;

    class Node {
    protected:
        UDPsocket socket_;
        
        // This computer's identity
        IPaddress host_;
        std::string hostname_;
        std::string ip_string_;

        // Receive packet
        Packet *packet_;
        UDPpacket *recv_;
        std::size_t packet_size_;

    public:
        Node(int socket_port, int packet_size);
        ~Node();

        // Get address
        uint32_t get_ip();

        // Get address as a domain string (i.e. XXX.XX.XX.X)
        std::string get_ip_string();

        // Get host name (normally the name on the computer)
        std::string get_hostname();

        // Get the received packet
        Packet *get_packet();

        // Send to a particular address, returns bytes sent
        int send_to(IPaddress *target, void *data, int len, int protocol);
        
        // Listen for incoming packets
        int listen();

        // Update the connector at the same rate as the rest of the application
        virtual bool update(unsigned dt) = 0;
    };

    // Convert 32-bit integer to human readable IP addresss
    std::string ip_to_string(uint32_t ipv4);
}

#endif