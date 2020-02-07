#include "node.h"

namespace Dynamo::Net {
    Node::Node(int socket_port, int packet_size) {
        // Initialize SDL_net
        if(SDLNet_Init() == -1) {
            throw GenericError(SDLNet_GetError());
        }

        // Open the socket
        socket_ = SDLNet_UDP_Open(socket_port);
        if(!socket_) {
            throw GenericError(SDLNet_GetError());
        }

        // Get local address information
        host_ = {};
        hostname_ = std::string(SDLNet_ResolveIP(&host_));
        if(SDLNet_ResolveHost(&host_, hostname_.c_str(), 0) == -1) {
            throw GenericError(SDLNet_GetError());
        }
        ip_string_ = ip_to_string(host_.host);

        // Allocate capture packets
        packet_ = new Packet(packet_size);
        recv_ = SDLNet_AllocPacket(packet_size);
        packet_size_ = packet_size;
    }

    Node::~Node() {
        delete packet_;
        SDLNet_FreePacket(recv_);
        SDLNet_UDP_Close(socket_);
        SDLNet_Quit();
    }

    uint32_t Node::get_ip() {
        return host_.host;
    }

    std::string Node::get_ip_string() {
        return ip_string_;
    }

    std::string Node::get_hostname() {
        return hostname_;
    }

    Packet *Node::get_packet() {
        return packet_;
    }

    int Node::send_to(IPaddress *target, void *data, int len, int protocol) {
        std::size_t header = sizeof(int);
        if(header + len > packet_size_) {
            throw PacketOverflow(packet_size_, len);
        }
        UDPpacket *packet = SDLNet_AllocPacket(packet_size_);

        packet->len = header + len;
        packet->address.host = target->host;
        packet->address.port = target->port;

        // Format the data
        std::memcpy(packet->data, &protocol, header);
        if(data) {
            std::memcpy(packet->data + header, data, len);
        }

        int bytes_sent = SDLNet_UDP_Send(socket_, -1, packet);
        SDLNet_FreePacket(packet);
        return bytes_sent;
    }

    int Node::listen() {
        std::size_t bytes = SDLNet_UDP_Recv(socket_, recv_);
        std::size_t header = sizeof(int);
        std::size_t size = recv_->len - header;
        
        if(bytes) {
            // Copy data to formatted packet for interfacing
            std::memcpy(&packet_->protocol, recv_->data, header);
            std::memcpy(packet_->data, recv_->data + header, size);
            packet_->size = size;
            packet_->source.host = recv_->address.host;
            packet_->source.port = recv_->address.port;
        }
        return bytes;
    }

    std::string ip_to_string(uint32_t ipv4) {
        unsigned char bytes[4];

        bytes[0] = ipv4 & 0xFF;
        bytes[1] = (ipv4 >> 8) & 0xFF;
        bytes[2] = (ipv4 >> 16) & 0xFF;
        bytes[3] = (ipv4 >> 24) & 0xFF;

        std::string ip_string = "";
        for(int i = 0; i < 4; i++) {
            ip_string += std::to_string((int)bytes[i]);
            if(i < 3) {
                ip_string += ".";
            }
        }
        return ip_string;
    }
}