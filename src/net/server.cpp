#include "server.h"

namespace Dynamo::Net {
    Server::Server(int port, int packet_size, int timeout_s) 
    : Node(port, packet_size) {
        port_ = static_cast<uint16_t>(port);
        timeout_ = timeout_s;
        time_ = 0;
    }

    int Server::get_port() {
        return port_;
    }

    int Server::get_num_clients() {
        return connected_.size();
    }

    bool Server::is_connected(uint32_t address) {
        return connected_.find(address) != connected_.end();
    }

    void Server::cull_clients() {
        auto it = connected_.begin();
        while(it != connected_.end()) {
            if(time_ - it->second.timestamp > timeout_ * 1000) {
                it = connected_.erase(it);
            }
            else {
                it++;
            }
        }
    }

    void Server::add_client(IPaddress *address) {
        connected_[address->host] = {
            address->port, 
            time_
        };
        send_to(address, nullptr, 0, NET_CONNECT);
    }

    void Server::reset_client(IPaddress *address) {
        connected_[address->host].timestamp = time_;
        send_to(address, nullptr, 0, NET_ALIVE);
    }

    void Server::send_all(void *data, int len, int protocol) {
        for(auto &conn : connected_) {
            IPaddress address = {
                conn.first,
                conn.second.port
            };
            send_to(&address, data, len, protocol);
        }
    }

    bool Server::update(unsigned dt) {
        time_ += dt;
        cull_clients();
        
        // Handle default protocols
        bool new_packet = listen();
        if(new_packet) {
            bool member = is_connected(packet_->source.host);
            if(!member) {
                if(packet_->protocol == NET_CONNECT) {
                    add_client(&packet_->source);
                    member = true;
                }
            }
            else {
                if(packet_->protocol == NET_ALIVE) {
                    reset_client(&packet_->source);
                }
                else if(packet_->protocol == NET_DISCONNECT) {
                    connected_.erase(packet_->source.host);
                    member = false;
                }
            }
            return member;
        }
        return new_packet;
    }
}