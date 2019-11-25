#ifndef DYNAMO_NET_SERVER_H_
#define DYNAMO_NET_SERVER_H_

#include <SDL2/SDL_net.h>

#include <unordered_map>
#include <utility>

#include "node.h"
#include "packet.h"

namespace Dynamo::Net {
    struct ClientMeta {
        uint16_t port;
        int timestamp;
        int secure_salt;
    };

    class Server : public Node {
        uint16_t port_;
        int timeout_;
        int time_;

        // Each address is mapped to a pair (port, timer)
        std::unordered_map<uint32_t, ClientMeta> connected_;

    protected:
        // Add a new client
        void add_client(IPaddress *address);

        // Reset the timestamp of a client
        void reset_client(IPaddress *address);

        // Remove all clients that are beyond timeout
        void cull_clients();

    public:
        Server(int port, int packet_size, int timeout_s);

        // Get the port
        int get_port();

        // Get the number of connected clients
        int get_num_clients();

        // Check if a client is connected
        bool is_connected(uint32_t address);

        // Send to all connected clients
        void send_all(void *data, int len, int protocol);

        // Update the server
        bool update(int dt);
    };
}

#endif