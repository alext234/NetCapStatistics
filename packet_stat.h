#ifndef __PACKET_STAT__
#define __PACKET_STAT__

#include "cpppcap.h"

using namespace Pcap;
class PacketStat:public AbstractObserver<Packet> {
public:
    void print () {
        // TODO print out the statistics
    }
private:
    void onNotified(const Packet& packet) override { // packet received
    }
    
};

#endif // __PACKET_STAT__ 
