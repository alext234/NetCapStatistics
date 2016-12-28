#ifndef __GROUP_STAT__
#define __GROUP_STAT__

#include "hostgroup.h"
#include "host_stat.h"
#include <memory>
#include "metric.h"

class GroupStat: public HostGroup, public AbstractObserver<Metric>, public std::enable_shared_from_this<GroupStat> {
public:
    GroupStat(std::string name) : HostGroup(name) {}
    virtual void addHost(std::shared_ptr<Hostipv4> host) override;
    const Metric& retrieve();
    
    using GroupSet = set<shared_ptr<GroupStat>>;
    using GroupSetPtr = shared_ptr<GroupSet>;
    static const GroupSet& getGroupsOfHost (std::shared_ptr<HostStat> host);
private:
    void onNotified (const Metric& update) override;
    Metric m;
    static void updateGroupList (std::shared_ptr<HostStat> host, std::shared_ptr<GroupStat> group);
};
#endif //__GROUP_STAT__
