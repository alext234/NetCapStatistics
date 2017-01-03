#ifndef __SUMMARY__
#define __SUMMARY__
#include <iostream>
#include <string>
#include "group_stat.h"
#include "host_stat.h"
#include "group_file_parser.h"
#include "packet_stat.h"

class Summary {
public:
    Summary( std::shared_ptr<PacketStat>& packetStat,
    GroupFileParser::SetPtrHostGroup allGroups,
    GroupFileParser::MapIpToHost mapIpToHost
    ) 
    : 
    packetStat {packetStat},
    allGroups {allGroups},
    mapIpToHost {mapIpToHost}

    {}



    std::string eachTxRxBytes();

private:
    std::shared_ptr<PacketStat>& packetStat;
    GroupFileParser::SetPtrHostGroup allGroups;
    GroupFileParser::MapIpToHost mapIpToHost;
};
#endif //__SUMMARY__
