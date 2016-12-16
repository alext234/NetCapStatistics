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
        // TODO get source and destination ip address (if there is)
        // 
    }
    
};

#endif // __PACKET_STAT__ 
