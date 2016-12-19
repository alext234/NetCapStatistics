#ifndef __HOST_GROUP__
#define __HOST_GROUP__

#include "hostip.h"
#include <set>
#include <memory>

struct shared_ptr_host_ipv4_compare {
    bool operator() (const std::shared_ptr<Hostipv4>& lhs, const std::shared_ptr<Hostipv4>& rhs) const{
        return *lhs < *rhs;
    }
};

class HostGroup {
public:
    HostGroup(std::string name): name(name) {}
    void addHost (std::shared_ptr<Hostipv4> host) {
       hostSet.insert(host); 
    }
    std::shared_ptr<Hostipv4> find (std::string hostname) {
        auto tempHostIp = std::make_shared<Hostipv4> ("0.0.0.0");
        tempHostIp->setHostname(hostname);
        auto it = hostSet.find(tempHostIp);
        if (it==hostSet.end()) return nullptr;
        return *it;
    }
    size_t size() { return hostSet.size();}
    std::string getName () {return name;}
    auto getHosts() {return hostSet;}
protected:
    std::string name;
    std::set<std::shared_ptr<Hostipv4>, shared_ptr_host_ipv4_compare> hostSet;
    
};


#endif // __HOST_GROUP__
