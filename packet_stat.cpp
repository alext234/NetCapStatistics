#include "packet_stat.h"
#include "hostip.h"
#include <set>
#include <memory>

using namespace Pcap;
using namespace std;



const set<uint32_t>&  PacketStat::getListOfUnmappedIps() {return listOfUnmappedIps;}



enum {
    ETHERTYPE_IPV4 = 0x0800
};

static inline uint16_t ntohs(uint16_t n) { // NOTE: this may not be cross-platform
    return (n>>8) | (n<<8);
    
}
static inline uint32_t ntohl(uint32_t n) { // NOTE: this may not be cross-platform
    uint32_t d0 = n&0xff;
    uint32_t d1 = (n>>8)&0xff;
    uint32_t d2 = (n>>16)&0xff;
    uint32_t d3 = (n>>24)&0xff;

    return d3|(d2<<8)|(d1<<16)|(d0<<24);
}
void PacketStat::onNotified(const Packet& packet) { // packet received
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
    updatePacketStat(ntohl(ip->src), ntohl(ip->dst), packet);



}
    

void PacketStat::trackUnmappedIp (uint32_t ip) {
    listOfUnmappedIps.insert(ip);

}
        

void PacketStat::updatePacketStat (uint32_t src, uint32_t dst, const Packet& packet) {
    auto len = packet.len();
    auto srcHost = mapIpToHost.find(src);
    auto dstHost = mapIpToHost.find(dst);
    if (srcHost!=mapIpToHost.end()) {
        auto h = static_pointer_cast<HostStat> (srcHost->second);
        h->add ({TxBytes(len),RxBytes(0)});
    } else {
        trackUnmappedIp(src);
    }

    if (dstHost!=mapIpToHost.end()) {
        auto h = static_pointer_cast<HostStat> (dstHost->second);
        h->add ({TxBytes(0),RxBytes(len)});
    } else {
        trackUnmappedIp(dst);
    }
}


    

