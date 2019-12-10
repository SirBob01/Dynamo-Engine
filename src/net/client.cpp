#include "client.h"
#include <iostream>

namespace Dynamo::Net {
    Client::Client(std::string ip, int port, int packet_size, int timeout_s) 
    : Node(0, packet_size) {
        server_ = {};
        if(SDLNet_ResolveHost(&server_, ip.c_str(), port) == -1) {
            throw SDLError(SDLNet_GetError());
        }
        server_ip_ = ip;
        
        time_ = 0;
        timestamp_ = 0; // Indicates no connection to server
        timeout_ = timeout_s;
    }

    uint32_t Client::get_server_ip() {
        return server_.host;
    }

    std::string Client::get_server_ip_string() {
        return server_ip_;
    }

    int Client::send(void *data, int len, int protocol) {
        return send_to(&server_, data, len, protocol);
    }

    bool Client::is_connected() {
        return !timestamp_;
    }

    bool Client::server_message() {
        if(listen()) {
            return packet_->source.host == server_.host;
        }
        return false;
    }

    bool Client::update(unsigned dt) {
        time_ += dt;
        bool comm_packet = server_message();

        if(time_ - timestamp_ > timeout_ * 1000) {
            // Terminate timed-out connection
            timestamp_ = 0;
            if(time_ % 2000 < 10) {
                std::cout << "Attempting connection...\n";
                send(nullptr, 0, NET_CONNECT);
            }
            if(comm_packet && packet_->protocol == NET_CONNECT) {
                std::cout << "Connected!";
                timestamp_ = time_;
            }
        }
        else {
            // TODO: This is very wonky. I'll work on this some other time
            // Send a status packet roughly every 2 seconds
            if(time_ % 2000 < 10) {
                std::cout << "Send status packet\n";
                send(nullptr, 0, NET_ALIVE);
            }
            if(comm_packet) {
                if(packet_->protocol == NET_ALIVE) {
                    timestamp_ = time_;
                }
                else if(packet_->protocol == NET_DISCONNECT) {
                    timestamp_ = 0;
                }
            }
        }
        return comm_packet;
    }
}