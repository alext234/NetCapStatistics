#ifndef __HOST_GROUP__
#define __HOST_GROUP__

#include "hostip.h"
#include <set>
class HostGroup {
public:
    HostGroup(std::string name): name(name) {}
    void addHost (const Hostipv4& host) {
       hostSet.insert(host); 
    }
    size_t size() { return hostSet.size();}
private:
    std::string name;
    std::set<std::reference_wrapper<const Hostipv4>> hostSet;
    
};


#endif // __HOST_GROUP__
