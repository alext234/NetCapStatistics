#include "host_stat.h"

using namespace std;



void HostStat::add(shared_ptr<HostStat> peer, Metric a) {
    m.add(a);
    HostStatNotifiedData data; 
    data.m=a;
    // TODO: add peer to data structure
    // TODO: track peer 
    notifyObservers (data); 
}

void HostStat::add(Metric a) {
    add(nullptr, a); 
}

const Metric & HostStat::retrieve() {
    return m;
}


HostStat::~HostStat() {
}


