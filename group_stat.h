#ifndef __GROUP_STAT__
#define __GROUP_STAT__

#include "hostgroup.h"
#include "host_stat.h"
#include <memory>
#include "metric.h"
#include "shared_ptr_cmp.h"
#include "counter_part.h"

class GroupStat: public HostGroup, public AbstractObserver<HostStatNotifiedData>, public std::enable_shared_from_this<GroupStat> {
public:
    GroupStat(std::string name) : HostGroup(name) {}
    virtual void addHost(std::shared_ptr<Hostipv4> host) override;
    const Metric& retrieve();
    
    const CounterPartList<GroupStat>& getPeerList();
    using GroupSet = set<shared_ptr<GroupStat>, shared_ptr_less<GroupStat>>;
    using GroupSetPtr = shared_ptr<GroupSet>;
    static const GroupSet& getGroupsOfHost (std::shared_ptr<HostStat> host);
    friend bool operator==(const GroupStat&, const GroupStat&);
    
private:
    void onNotified (const HostStatNotifiedData& update) override;
    Metric m;
    CounterPartList<GroupStat> peerList;
    static void updateGroupList (std::shared_ptr<HostStat> host, std::shared_ptr<GroupStat> group);
};
#endif //__GROUP_STAT__
