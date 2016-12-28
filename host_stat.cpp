#include "host_stat.h"

using namespace std;


void HostStat::incTxBytes (uint32_t n) { 
    add({n,0});
}

void HostStat::incRxBytes (uint32_t n) { 
    add({0,n});
}

void HostStat::add(Metric a) {
    m.add(a);
    notifyObservers (a);
    
}

HostStat::~HostStat() {
}

uint64_t HostStat::getRxBytes() { return m.rxBytes;}
uint64_t HostStat::getTxBytes() { return m.txBytes;}


