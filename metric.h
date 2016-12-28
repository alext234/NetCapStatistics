#ifndef __METRIC__
#define __METRIC__

#include <cstdint>
struct Metric {   
    uint64_t  txBytes=0;
    uint64_t  rxBytes=0;
    Metric(){}
    Metric(uint64_t tx, uint64_t rx) : txBytes{tx}, rxBytes{rx} {}
    void add(const Metric& a) { txBytes +=a.txBytes; rxBytes+=a.rxBytes; }
};


#endif //__METRIC__
