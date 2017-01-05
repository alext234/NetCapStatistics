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

void GroupStat::onNotified (const HostStatNotifiedData& update) {
    m.add(update.m);

    // update group's peers 
    Metric peerAdd; 
    peerAdd.txBytes = update.m.txBytes;
    peerAdd.rxBytes = update.m.rxBytes;

    

    if (update.peer!=nullptr) {
        auto it1 = hostSet.find(update.peer); 
        if (it1!=hostSet.end() ) { // peer also exist in this group
            return;
        }
        // get the groups of the peer
   
        auto peerGroups = getGroupsOfHost (update.peer);
         
        // iterate through the group,
        for (auto group: peerGroups) {
            if (!(*group == *this)) {
                auto groupHosts = group->getHosts();
                auto it2 = groupHosts.find(make_shared<HostStat>(getIpString(*(update.host)))); 
                if (it2!=groupHosts.end()) { 
                    continue; // host should not be in peer's group
                }
                // if it is different from this group then add to peer
                peerList.add (group,  peerAdd);

            }
        }


    }
    
    

}

const Metric& GroupStat::retrieve() {
    return m;
}

const CounterPartList<GroupStat>& GroupStat::getPeerList(){
    return peerList;
}

using GroupSet = GroupStat::GroupSet;
using GroupSetPtr = GroupStat::GroupSetPtr;

using HostMapOfGroupSetPtr = map<shared_ptr<HostStat>, GroupSetPtr, shared_ptr_less<Hostipv4>>;
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

bool operator==(const GroupStat& lhs, const GroupStat& rhs){
    return lhs.name == rhs.name;
}
