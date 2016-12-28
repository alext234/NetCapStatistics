#ifndef __HOST_GROUP__
#define __HOST_GROUP__

#include "hostip.h"
#include <set>
#include <memory>
#include "shared_ptr_cmp.h"

class HostGroup{
public:
    HostGroup(std::string name): name(name) {}
    virtual ~HostGroup() {}
    virtual void addHost (std::shared_ptr<Hostipv4> host) ;

    std::shared_ptr<Hostipv4> find (std::string hostname) ;

    size_t size() { return hostSet.size();}
    std::string getName () {return name;}
    std::set<std::shared_ptr<Hostipv4>, shared_ptr_less<Hostipv4> >  getHosts();

    // comparison for set operation      
    friend bool operator< (const HostGroup &left, const HostGroup &right);
     
protected:
    std::string name;
    std::set<std::shared_ptr<Hostipv4>,  shared_ptr_less<Hostipv4>   > hostSet;
    
};

#endif // __HOST_GROUP__
