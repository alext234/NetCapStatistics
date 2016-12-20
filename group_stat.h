#ifndef __GROUP_STAT__
#define __GROUP_STAT__

#include "hostgroup.h"

class GroupStat: public HostGroup {
public:
    GroupStat(std::string name) : HostGroup(name) {}
    virtual void addHost(std::shared_ptr<Hostipv4> host) override;
};
#endif //__GROUP_STAT__
