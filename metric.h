#ifndef __METRIC__
#define __METRIC__

#include <cstdint>

enum class TxBytes: uint64_t;
enum class RxBytes: uint64_t;

struct Metric {   
    uint64_t  txBytes=0;
    uint64_t  rxBytes=0;
    Metric(){}
    Metric(TxBytes tx, RxBytes rx) : 
        txBytes{static_cast<uint64_t>(tx)}, 
        rxBytes{static_cast<uint64_t>(rx)}
        {}
    void add(const Metric& a) { txBytes +=a.txBytes; rxBytes+=a.rxBytes; }
};


#endif //__METRIC__
