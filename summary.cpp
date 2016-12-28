#include "summary.h"
#include <iostream>

using namespace std;
string ipString (uint32_t ip) {
    stringstream ss;

    ss << ((ip>>24)&0xff) <<".";
    ss << ((ip>>16)&0xff) <<".";
    ss << ((ip>>8)&0xff) <<".";
    ss << ((ip)&0xff);
    return ss.str();
}


string Summary::summaryString() {
    stringstream ss;

    auto listOfUnmappedIps = packetStat->getListOfUnmappedIps();
    if (listOfUnmappedIps.size() >0) {
        ss<<"these IP's are not mapped to hostname :" <<endl;
        for (auto ip: listOfUnmappedIps) { ss<<ipString(ip)<<endl;    }
    }

    ss<<dec;
    ss.imbue(std::locale("")); //  // imbue global locale ; for comma separated numbers

    // output groups
    for (auto group: allGroups) { 
        auto groupStat = static_pointer_cast<GroupStat>(group);

        ss << groupStat->getName() <<":"<<endl;
        auto groupMetric = groupStat->retrieve();
        ss <<'\t'<<"Tx: "<<groupMetric.txBytes<<" bytes"<<endl;
        ss <<'\t'<<"Rx: "<<groupMetric.rxBytes<<" bytes"<<endl;
    }
   
    // output hosts
    for (auto it=mapIpToHost.cbegin(); it!=mapIpToHost.cend(); ++it) {
        auto host = static_pointer_cast<HostStat>(it->second);
        ss << host->getHostname() <<" (" <<host->getIpString()<<"):"<<endl;
        auto hostMetric = host->retrieve();
        ss <<'\t'<<"Tx: "<<hostMetric.txBytes<<" bytes"<<endl;
        ss <<'\t'<<"Rx: "<<hostMetric.rxBytes<<" bytes"<<endl;
    }



   return ss.str(); 

}

// TODO: output csv
// TODO: output host of each group
// TODO: option to not output host that has tx=0 and rx=0

