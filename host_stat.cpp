#include "host_stat.h"

using namespace std;


void HostStat::add(shared_ptr<HostStat> peer, Metric a) {
    m.add(a);
    HostStatNotifiedData data; 
    data.m=a;
    if (peer!=nullptr) {
        Metric peerAdd;
        peerAdd.txBytes = a.rxBytes;
        peerAdd.rxBytes = a.txBytes;

        peerList.add (peer, peerAdd );

    }        

    // TODO: add peer to data structure before notify

    notifyObservers (data); 
}

void HostStat::add(Metric a) {
    add(nullptr, a); 
}

const Metric & HostStat::retrieve() {
    return m;
}


const CounterPartList<HostStat>& HostStat::getPeerList() { return peerList;}
HostStat::~HostStat() {
}


