#ifndef DYNAMO_NET_SERVER_H_
#define DYNAMO_NET_SERVER_H_

#include <unordered_map>
#include <utility>
#include <chrono>

#include "node.h"

namespace Dynamo::Net {
    struct ClientMeta {
        uint16_t port;
        long long int timestamp;
    };

    class Server : public Node {
    protected:
        uint16_t port_;
        int timeout_;

        // Each address is mapped to a pair (port, timer)
        std::unordered_map<uint32_t, ClientMeta> connected_;

    public:
        Server(int port, int packet_size, int timeout_s);

        // Get the port
        int get_port();

        // Get the number of connected clients
        int get_num_connected();

        // Reset the timer of a client
        bool is_connected(uint32_t address);

        // Remove all clients that are beyond timeout
        void cull_connected(long long int time);

        // Send to all connected clients
        void send_all(void *data, int len, int protocol);

        // Update the server
        bool tick();
    };
}

#endif