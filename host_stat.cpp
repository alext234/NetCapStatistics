#include "host_stat.h"

using namespace std;


void HostStat::add(shared_ptr<HostStat> peer, Metric a) {
    m.add(a);
    if (peer!=nullptr) {
        Metric peerAdd;
        peerAdd.txBytes = a.txBytes;
        peerAdd.rxBytes = a.rxBytes;

        peerList.add (peer, peerAdd );

    }        


    HostStatNotifiedData data; 
    data.m=a;
    data.host = this; //use raw pointer as using shared pointer may cause exception  when 'this' has not shared_pointer pointed to
    data.peer = peer; // can be nullptr 
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


