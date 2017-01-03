#include "hostgroup.h"

void HostGroup::addHost (std::shared_ptr<Hostipv4> host) {
    hostSet.insert(host); 

}
std::shared_ptr<Hostipv4> HostGroup::find (std::string hostname) {
    auto tempHostIp = std::make_shared<Hostipv4> ("0.0.0.0");
    tempHostIp->setHostname(hostname);
    auto it = hostSet.find(tempHostIp);    
    if (it==hostSet.end()) return nullptr;
    return *it;
}
std::set<std::shared_ptr<Hostipv4>,   shared_ptr_less<Hostipv4>>   HostGroup::getHosts() {
    return hostSet;
}


bool operator< (const HostGroup &left, const HostGroup &right) {
    return left.name< right.name;
} 
