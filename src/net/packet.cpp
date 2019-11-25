#include "packet.h"

namespace Dynamo::Net {
    Packet::Packet(int packet_size) {
        header = {0, 0, 0};

        data = new char[packet_size];
        size = 0;
        source = {};
    }

    Packet::~Packet() {
        delete data;
    }
}