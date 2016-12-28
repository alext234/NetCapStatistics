#ifndef __HOST_STAT__
#define __HOST_STAT__
#include "hostip.h"
#include "cpp_observer.h"
#include "metric.h"
using namespace std;


class GroupStat; // forward declaration
class HostStat: public Hostipv4, public Observable<Metric>  {
public:
    HostStat(string ip_string): Hostipv4(ip_string) {}
    ~HostStat();

    void incTxBytes (uint32_t n);
    void incRxBytes (uint32_t n);

    uint64_t getRxBytes();
    uint64_t getTxBytes();

private:
    Metric m;
    void add (Metric a);

    
    
};



#endif //__HOST_STAT__
