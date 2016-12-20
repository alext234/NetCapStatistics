#include "hostgroup.h"

bool shared_ptr_host_ipv4_compare::operator() (const std::shared_ptr<Hostipv4>& lhs, const std::shared_ptr<Hostipv4>& rhs) const
{
   return *lhs < *rhs;
}

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
std::set<std::shared_ptr<Hostipv4>, shared_ptr_host_ipv4_compare>   HostGroup::getHosts() {
    return hostSet;
}



