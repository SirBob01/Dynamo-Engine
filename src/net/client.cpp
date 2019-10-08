#include "client.h"

namespace Dynamo::Net {
    Client::Client(std::string ip, int port, int packet_size, int timeout_s) 
    : Node(0, packet_size) {
        server_ = {};
        if(SDLNet_ResolveHost(&server_, ip.c_str(), port) == -1) {
            throw SDLError(SDLNet_GetError());
        }

        server_name_ = std::string(SDLNet_ResolveIP(&server_));
        server_ip_ = ip;

        timestamp_ = -1; // Indicates no connection to server
        timeout_ = timeout_s;
    }

    uint32_t Client::get_server_ip() {
        return server_.host;
    }

    std::string Client::get_server_ip_string() {
        return server_ip_;
    }

    std::string Client::get_server_name() {
        return server_name_;
    }

    int Client::send(void *data, int len, int protocol) {
        return send_to(&server_, data, len, protocol);
    }

    bool Client::tick() {
        auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
        );
        long long int current = time.time_since_epoch().count();

        bool server_update = listen();

        if(current - timestamp_ > timeout_ * 1000) {
            // 20 seconds, then terminate connection
            timestamp_ = 0;
        }
        else if(server_update) {
            if(packet_->protocol == NET_CONNECT || 
               packet_->protocol == NET_ALIVE) {
                timestamp_ = current;
            }
            else if(packet_->protocol == NET_DISCONNECT) {
                timestamp_ = 0;
            }
        }
        return server_update;
    }
}