#include "server.h"

namespace Dynamo::Net {
    Server::Server(int port, size_t packet_size) {
        socket_ = SDLNet_UDP_Open(port);
        if(!socket_) {
            throw SDLError(SDLNet_GetError());
        }

        if(SDLNet_ResolveHost(&host_, nullptr, port) == -1) {
            throw SDLError(SDLNet_GetError());
        }

        hostname_ = std::string(SDLNet_ResolveIP(&host_));
        if(SDLNet_ResolveHost(&host_, hostname_.c_str(), port) == -1) {
            throw SDLError(SDLNet_GetError());
        }

        uint32_t ipv4 = host_.host;
        unsigned char bytes[4];
        bytes[0] = ipv4 & 0xFF;
        bytes[1] = (ipv4 >> 8) & 0xFF;
        bytes[2] = (ipv4 >> 16) & 0xFF;
        bytes[3] = (ipv4 >> 24) & 0xFF;

        ip_string_ = "";
        for(int i = 0; i < 4; i++) {
            ip_string_ += std::to_string((int)bytes[i]);
            if(i < 3) {
                ip_string_ += ".";
            }
        }

        recv_ = SDLNet_AllocPacket(packet_size);
        packet_size_ = packet_size;
    }

    Server::~Server() {
        SDLNet_FreePacket(recv_);
        SDLNet_UDP_Close(socket_);
    }

    std::string Server::get_hostname() {
        return hostname_;
    }

    std::string Server::get_ip() {
        return ip_string_;
    }

    void Server::tick() {
        /* TODO: Check each client to see when it 
         * last sent a packet to server. Disconnect if over
         * certain timeout value.
         */
        listen();
    }

    bool Server::add_client(IPaddress address) {
        for(auto &conn : connected_) {
            if(conn.first.host == address.host) {
                return false; // This client is already connected
            }
        }
        connected_.push_back(std::make_pair(address, 0));
        return true;
    }

    bool Server::remove_client(IPaddress address) {
        for(int i = 0; i < connected_.size(); i++) {
            if(connected_[i].first.host == address.host) {
                connected_.erase(connected_.begin()+i);
                return true;
            }
        }
        return false;
    }

    bool Server::reset_client(IPaddress address) {
        for(auto &conn : connected_) {
            if(conn.first.host == address.host) {
                conn.second = 0;
                return true;
            }
        }
        return false;
    }

    int Server::send(IPaddress *target, void *data, int len, int protocol) {
        size_t header = sizeof(int);
        if(header + len > packet_size_) {
            throw PacketOverflow(packet_size_, len);
        }
        UDPpacket *packet = SDLNet_AllocPacket(packet_size_);

        // Format the data
        packet->len = header + len;
        packet->address.host = target->host;
        packet->address.port = target->port;

        std::memcpy(packet->data, &protocol, header);
        if(data) {
            std::memcpy(packet->data + header, data, len);
        }

        int bytes_sent = SDLNet_UDP_Send(socket_, -1, packet);
        SDLNet_FreePacket(packet);
        return bytes_sent;
    }

    void Server::send_all(void *data, int len, int protocol) {
        for(auto &conn : connected_) {
            send(&conn.first, data, len, protocol);
        }
    }

    int Server::listen() {
        size_t bytes = SDLNet_UDP_Recv(socket_, recv_);
        size_t header = sizeof(int);
        
        int protocol;
        if(bytes) {
            std::memcpy(&protocol, recv_->data, header);
            if(protocol == NET_CONNECT) {
                add_client(recv_->address);
            }
            else if(protocol == NET_DISCONNECT) {
                remove_client(recv_->address);
            }
            else if(protocol == NET_ALIVE) {
                reset_client(recv_->address);
            }
            else {
                action(
                    recv_->data + header, 
                    recv_->len - header, 
                    protocol
                );
            }
        }
        return bytes;
    }
}