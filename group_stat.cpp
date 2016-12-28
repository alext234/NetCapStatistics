#include "group_stat.h"
#include "host_stat.h"
#include <map>
#include <memory>

using namespace std;
void GroupStat::addHost(shared_ptr<Hostipv4> host) {
    HostGroup::addHost(host);

    auto hostStat = static_pointer_cast<HostStat> (host);
    hostStat->registerObserver (shared_from_this());

    updateGroupList (hostStat, shared_from_this());
}

void GroupStat::onNotified (const Metric& update) {
    m.add(update);

}

const Metric& GroupStat::retrieve() {
    return m;
}

using GroupSet = GroupStat::GroupSet;
using GroupSetPtr = GroupStat::GroupSetPtr;

using HostMapOfGroupSetPtr = map<shared_ptr<HostStat>, GroupSetPtr, shared_ptr_host_ipv4_compare>;
static HostMapOfGroupSetPtr hostGroupSet;
void GroupStat::updateGroupList (shared_ptr<HostStat> host, shared_ptr<GroupStat> group){
    auto ret = hostGroupSet.find(host);
    if (ret==hostGroupSet.end()) {
        
        hostGroupSet[host] = make_shared<GroupSet>();        

    }
    auto groupSet = hostGroupSet[host];
    
    groupSet->insert (group);
    
}
const GroupSet& GroupStat::getGroupsOfHost (std::shared_ptr<HostStat> host) {
    auto ret = hostGroupSet.find(host);
    if (ret==hostGroupSet.end()) {
        
        hostGroupSet[host] = make_shared<GroupSet>();        
    }
    return *hostGroupSet[host];
}

