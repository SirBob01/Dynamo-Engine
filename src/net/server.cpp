#include "server.h"

namespace Dynamo::Net {
    Server::Server(int port, int packet_size, int timeout_s) 
    : Node(port, packet_size) {
        port_ = static_cast<uint16_t>(port);
        timeout_ = timeout_s;
    }

    int Server::get_port() {
        return port_;
    }

    int Server::get_num_connected() {
        return connected_.size();
    }

    bool Server::is_connected(uint32_t address) {
        for(auto &conn : connected_) {
            if(conn.first == address) {
                return true;
            }
        }
        return false;
    }

    void Server::cull_connected(long long int time) {
        auto it = connected_.begin();
        while(it != connected_.end()) {
            if(time - it->second.timestamp > timeout_) {
                it = connected_.erase(it);
            }
            else {
                it++;
            }
        }
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

    bool Server::tick() {
        auto time = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
        );
        long long int current = time.time_since_epoch().count();

        cull_connected(current);
        
        // Handle default protocols
        bool new_packet = listen();
        if(new_packet) {
            if(!is_connected(packet_->source.port)) {
                if(packet_->protocol == NET_CONNECT) {
                    connected_[packet_->source.host] = {
                        packet_->source.port, 
                        current
                    };
                    return true;
                }
            }
            else {
                if(packet_->protocol == NET_ALIVE) {
                    connected_[packet_->source.host].timestamp = current;
                }
                else if(packet_->protocol == NET_DISCONNECT) {
                    connected_.erase(packet_->source.host);
                }
                return true;
            }
        }
        return false;
    }
}