#ifndef __PACKET_STAT__
#define __PACKET_STAT__

#include "cpppcap.h"
#include "hostip.h"
#include "host_stat.h"
#include <set>
#include <map>

using namespace Pcap;
using namespace std;



class PacketStat:public AbstractObserver<Packet> {
public:
    PacketStat (map<uint32_t, shared_ptr<Hostipv4>>& mapIpToHost) : mapIpToHost{mapIpToHost} {
    }

    const set<uint32_t>&  getListOfUnmappedIps();

private:
    map<uint32_t, shared_ptr<Hostipv4>>&  mapIpToHost;
    set<uint32_t> listOfUnmappedIps;

    void onNotified(const Packet& packet) override;
    void trackUnmappedIp (uint32_t ip) ;
    void updatePacketStat (uint32_t src, uint32_t dst, const Packet& packet);

};

#endif // __PACKET_STAT__ 
