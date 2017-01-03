#include "summary.h"
#include <iostream>
#include <iomanip>

using namespace std;
string ipString (uint32_t ip) {
    stringstream ss;

    ss << ((ip>>24)&0xff) <<".";
    ss << ((ip>>16)&0xff) <<".";
    ss << ((ip>>8)&0xff) <<".";
    ss << ((ip)&0xff);
    return ss.str();
}

static vector<shared_ptr<GroupStat>> getListOfSortedGroups(GroupFileParser::SetPtrHostGroup allGroups) {

    vector<shared_ptr<GroupStat>> sortedGroups ;
    for (auto group: allGroups) {
        auto groupStat = static_pointer_cast<GroupStat>(group);
        sortedGroups.push_back(groupStat);
    }
    sort (sortedGroups.begin(), sortedGroups.end(), 
    [](shared_ptr<GroupStat> lhs, shared_ptr<GroupStat> rhs)->bool {
        auto lhsMetric = lhs->retrieve();
        auto rhsMetric = rhs->retrieve();
        return lhsMetric.txBytes+lhsMetric.rxBytes > rhsMetric.txBytes+rhsMetric.rxBytes;
    });
    return sortedGroups;
}
static vector<shared_ptr<HostStat>> getListOfSortedHosts(GroupFileParser::MapIpToHost mapIpToHost) {

    vector<shared_ptr<HostStat>> sortedHosts ;
    for (auto it=mapIpToHost.cbegin(); it!=mapIpToHost.cend(); ++it) {
        auto host = static_pointer_cast<HostStat>(it->second);
        sortedHosts.push_back(host);
    }
    sort (sortedHosts.begin(), sortedHosts.end(), 
    [](shared_ptr<HostStat> lhs, shared_ptr<HostStat> rhs)->bool {
        auto lhsMetric = lhs->retrieve();
        auto rhsMetric = rhs->retrieve();
        return lhsMetric.txBytes+lhsMetric.rxBytes > rhsMetric.txBytes+rhsMetric.rxBytes;
    });
    return sortedHosts;
}

static const string SEP="|";
static void setupStringStream(stringstream& ss) {
    ss<<dec;
    ss.imbue(std::locale("")); //  // imbue global locale ; for comma separated numbers
}

string Summary::eachTxRxBytes() {
    stringstream ss;

    auto listOfUnmappedIps = packetStat->getListOfUnmappedIps();
    if (listOfUnmappedIps.size() >0) {
        ss<<"these IP's are not mapped to hostname :" <<endl;
        for (auto ip: listOfUnmappedIps) { ss<<ipString(ip)<<endl;    }
    }

    setupStringStream(ss);
    ss<<setw(20)<<"name"<<SEP<<setw(30)<<"TXbytes" << SEP<<setw(30) <<"RXbytes"<<endl; // colum header 
    // output groups
    for (auto groupStat: getListOfSortedGroups(allGroups)) { 

        auto metric = groupStat->retrieve();
        ss << setw(20)<<groupStat->getName()<<SEP;
        ss << setw(30)<< metric.txBytes<<SEP;
        ss << setw(30)<<metric.rxBytes<<endl;
    }
   
    // output hosts
    for (auto hostStat: getListOfSortedHosts(mapIpToHost)) { 

        auto metric = hostStat->retrieve();
        ss << setw(20)<<hostStat->getHostname()<<SEP;
        ss << setw(30)<< metric.txBytes<<SEP;
        ss << setw(30)<< metric.rxBytes<<endl;
    }
   



   return ss.str(); 

}


string Summary::groupPeers() {
    stringstream ss;

    setupStringStream(ss);
    ss<<setw(20)<<"peer" << SEP<<setw(30) <<"TXbytes (to peer)"<<SEP<<setw(30)<<"Rxbytes (from peer)"<<endl; // colum header 
    for (auto groupStat: getListOfSortedGroups(allGroups)) { 
        ss<<"Peer(s) of "<<groupStat->getName()<<":"<<endl;
        auto allPeers = groupStat->getPeerList().retrieveAll();
        for (auto it=allPeers.cbegin(); it!=allPeers.cend(); ++it) {
            auto peerGroup  = it->first;
            auto peerMetric  = it->second;
            ss<<setw(20)<<peerGroup->getName() << SEP<<setw(30) <<peerMetric->txBytes<<SEP<<setw(30)<<peerMetric->rxBytes<<endl;
    
        }

    }

    return ss.str();
}

string Summary::hostPeers() {
    stringstream ss;

    setupStringStream(ss);

    ss<<setw(15)<<"peer" << SEP<<setw(30) <<"TXbytes (to peer)"<<SEP<<setw(30)<<"Rxbytes (from peer)"<<endl; // colum header 
    for (auto hostStat: getListOfSortedHosts(mapIpToHost)) { 
        ss<<"Peer(s) of "<< hostStat->getHostname()<<":"<<endl;
        auto allPeers = hostStat->getPeerList().retrieveAll();
        for (auto it=allPeers.cbegin(); it!=allPeers.cend(); ++it) {
            auto peerHost = it->first;
            auto peerMetric  = it->second;
            ss<<setw(15)<<peerHost->getHostname() << SEP<<setw(30) <<peerMetric->txBytes<<SEP<<setw(30)<<peerMetric->rxBytes<<endl;
    
        }

    }

    return ss.str();
}
