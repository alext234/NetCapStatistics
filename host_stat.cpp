#include "host_stat.h"

using namespace std;


void HostStat::incTxBytes (uint32_t n) { 
    txBytes+=n;
}

void HostStat::incRxBytes (uint32_t n) { 
        rxBytes+=n;
}



uint64_t HostStat::getRxBytes() { return rxBytes;}
uint64_t HostStat::getTxBytes() { return txBytes;}


