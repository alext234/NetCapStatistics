#include "host_stat.h"

using namespace std;



void HostStat::add(Metric a) {
    m.add(a);
    notifyObservers (a);
    
}

const Metric & HostStat::retrieve() {
    return m;
}
    

HostStat::~HostStat() {
}


