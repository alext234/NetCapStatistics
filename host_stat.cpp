#include "host_stat.h"

using namespace std;


void HostStat::incTxBytes (uint32_t n) { 
    incTxRxBytes({n,0});
}

void HostStat::incRxBytes (uint32_t n) { 
    incTxRxBytes({0,n});
}

void HostStat::incTxRxBytes(IncTxRx incTxRx) {
    txBytes +=incTxRx.tx;
    rxBytes +=incTxRx.rx;
    notifyObservers (incTxRx);
    
}

HostStat::~HostStat() {
}

uint64_t HostStat::getRxBytes() { return rxBytes;}
uint64_t HostStat::getTxBytes() { return txBytes;}


