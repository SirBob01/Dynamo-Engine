#ifndef DYNAMO_NET_PACKET_H_
#define DYNAMO_NET_PACKET_H_

#include <SDL2/SDL_net.h>

namespace Dynamo::Net {
    struct AckHead {
        unsigned int id;
        unsigned int seq;
        unsigned int ack;
        uint16_t bitfield;
    };

    // Formatted packet
    struct Packet {
        AckHead header;
        short protocol;

        char *data;
        int size;
        IPaddress source;

        Packet(int packet_size);
        ~Packet();
    };
}

#endif