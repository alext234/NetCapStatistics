#ifndef __PACKET_STAT__
#define __PACKET_STAT__

#include "cpppcap.h"
#include "hostip.h"
#include <set>

using namespace Pcap;
using namespace std;

class HostStat: public Hostipv4  {
public:
    HostStat(string ip_string): Hostipv4(ip_string), rxBytes{0}, txBytes{0} {}

    inline void incTxBytes (uint32_t n) { txBytes+=n;}
    inline void incRxBytes (uint32_t n) { rxBytes+=n;}

    void incTxRxBytes (uint32_t tx, uint32_t rx) {
        txBytes +=tx;
        rxBytes +=rx;
    }

    pair<uint64_t, uint64_t> getTxRxBytes () {
        return make_pair(txBytes, rxBytes);
    }
private:
    uint64_t rxBytes;
    uint64_t txBytes;
    
};


class PacketStat:public AbstractObserver<Packet> {
public:
    PacketStat (map<uint32_t, shared_ptr<HostStat>>& mapIpToHost) : mapIpToHost{mapIpToHost} {
    }

    auto getListOfUnmappedIps() {return listOfUnmappedIps;}

private:
    map<uint32_t, shared_ptr<HostStat>>&  mapIpToHost;
    set<uint32_t> listOfUnmappedIps;

    enum {
        ETHERTYPE_IPV4 = 0x0800
    };

    void onNotified(const Packet& packet) override { // packet received
        #pragma pack(push,1) // exact fit, no padding
        struct ether_header_t {
            uint8_t dst[6];
            uint8_t src[6];
            uint16_t ether_type;
        };
        
        auto dataVector = packet.data(); // in vector of uint8_t
        auto rawData = &dataVector[0];
        auto ether = (ether_header_t*)rawData;
        if (ntohs(ether->ether_type) != ETHERTYPE_IPV4) return;

        // only handle IPV4
        auto ipv4RawData = rawData + sizeof(ether_header_t);

        #pragma pack(push,1) 
        struct ipv4_hdr_t {
           uint8_t unused[12];
           uint32_t src;
           uint32_t dst;
        };
        auto ip = (ipv4_hdr_t*) ipv4RawData;
       
        //now we have ip->sr, ip->dst, and packet.len()
        updatePacketStat(ip->src, ip->dst, packet);



    }
    
    inline uint16_t ntohs(uint16_t n) { // NOTE: this may not be cross-platform
        return (n>>8) | (n<<8);
        
    }

    void updatePacketStat (uint32_t src, uint32_t dst, const Packet& packet) {
        auto len = packet.len();
        auto srcHost = mapIpToHost.find(src);
        auto dstHost = mapIpToHost.find(dst);
        if (srcHost!=mapIpToHost.end()) {
            srcHost->second->incTxBytes(len);
        } else {
            trackUnmappedIp(src);
        }

        if (dstHost!=mapIpToHost.end()) {
            dstHost->second->incRxBytes (len);
        } else {
            trackUnmappedIp(dst);
        }
    }

    void trackUnmappedIp (uint32_t ip) {
        listOfUnmappedIps.insert(ip);

    }

    
};

#endif // __PACKET_STAT__ 
