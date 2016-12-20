#include "group_stat.h"
#include "host_stat.h"

#include <memory>

using namespace std;
void GroupStat::addHost(shared_ptr<Hostipv4> host) {
    HostGroup::addHost(host);

    auto hostStat = static_pointer_cast<HostStat> (host);
    hostStat->registerObserver (shared_from_this());


}
void GroupStat::onNotified (const IncTxRx& update) {
    totalTxBytes +=update.tx;
    totalRxBytes +=update.rx;
}
uint64_t GroupStat::getTotalTxBytes () { return totalTxBytes;}
uint64_t GroupStat::getTotalRxBytes () { return totalRxBytes;}
