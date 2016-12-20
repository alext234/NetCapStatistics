#ifndef __HOST_GROUP__
#define __HOST_GROUP__

#include "hostip.h"
#include <set>
#include <memory>

struct shared_ptr_host_ipv4_compare {
    bool operator() (const std::shared_ptr<Hostipv4>& lhs, const std::shared_ptr<Hostipv4>& rhs)const;
};

class HostGroup {
public:
    HostGroup(std::string name): name(name) {}
    virtual ~HostGroup() {}
    virtual void addHost (std::shared_ptr<Hostipv4> host) ;

    std::shared_ptr<Hostipv4> find (std::string hostname) ;

    size_t size() { return hostSet.size();}
    std::string getName () {return name;}
    std::set<std::shared_ptr<Hostipv4>, shared_ptr_host_ipv4_compare>  getHosts();
protected:
    std::string name;
    std::set<std::shared_ptr<Hostipv4>, shared_ptr_host_ipv4_compare> hostSet;
    
};


#endif // __HOST_GROUP__
