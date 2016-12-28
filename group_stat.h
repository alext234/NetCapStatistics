#ifndef __GROUP_STAT__
#define __GROUP_STAT__

#include "hostgroup.h"
#include "host_stat.h"
#include <memory>
class GroupStat: public HostGroup, public AbstractObserver<IncTxRx>, public std::enable_shared_from_this<GroupStat> {
public:
    GroupStat(std::string name) : HostGroup(name) {}
    virtual void addHost(std::shared_ptr<Hostipv4> host) override;
    uint64_t getTotalTxBytes ();
    uint64_t getTotalRxBytes ();
    
    using GroupSet = set<shared_ptr<GroupStat>>;
    using GroupSetPtr = shared_ptr<GroupSet>;
    static const GroupSet& getGroupsOfHost (std::shared_ptr<HostStat> host);
private:
    void onNotified (const IncTxRx& update) override;
    uint64_t totalTxBytes=0;
    uint64_t totalRxBytes=0;
    static void updateGroupList (std::shared_ptr<HostStat> host, std::shared_ptr<GroupStat> group);
};
#endif //__GROUP_STAT__
