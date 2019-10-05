#include "client.h"

namespace Dynamo::Net {
    Client::Client(std::string ip, int port, size_t packet_size) {
        socket_ = SDLNet_UDP_Open(0);
        if(!socket_) {
            throw SDLError(SDLNet_GetError());
        }
        if(SDLNet_ResolveHost(&host_, ip.c_str(), port) == -1) {
            throw SDLError(SDLNet_GetError());
        }
        hostname_ = std::string(SDLNet_ResolveIP(&host_));
        ip_string_ = ip;

        recv_ = SDLNet_AllocPacket(packet_size);
        packet_size_ = packet_size;

        connected_ = false;
    }

    Client::~Client() {
        SDLNet_FreePacket(recv_);
        SDLNet_UDP_Close(socket_);
    }

    std::string Client::get_hostname() {
        return hostname_;
    }

    std::string Client::get_ip() {
        return ip_string_;
    }

    void Client::tick() {
        // TODO: Periodically send server NET_ALIVE packets
        listen();
    }

    int Client::send(void *data, int len, int protocol) {
        size_t header = sizeof(int);
        if(header + len > packet_size_) {
            throw PacketOverflow(packet_size_, len);
        }
        UDPpacket *packet = SDLNet_AllocPacket(packet_size_);

        // Format the data
        packet->len = header + len;
        packet->address.host = host_.host;
        packet->address.port = host_.port;

        std::memcpy(packet->data, &protocol, header);
        if(data) {
            std::memcpy(packet->data + header, data, len);
        }

        int bytes_sent = SDLNet_UDP_Send(socket_, -1, packet);
        SDLNet_FreePacket(packet);
        return bytes_sent;
    }

    int Client::listen() {
        size_t bytes = SDLNet_UDP_Recv(socket_, recv_);
        size_t header = sizeof(int);
        
        int protocol;
        if(bytes) {
            std::memcpy(&protocol, recv_->data, header);
            if(protocol == NET_CONNECT) {
                connected_ = true;
            }
            else if(protocol == NET_DISCONNECT) {
                connected_ = false;
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