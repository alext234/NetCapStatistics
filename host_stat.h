#ifndef __HOST_STAT__
#define __HOST_STAT__
#include "hostip.h"
#include "cpp_observer.h"

using namespace std;

struct IncTxRx {
    uint32_t tx;
    uint32_t rx;
};

class GroupStat; // forward declaration
class HostStat: public Hostipv4, public Observable<IncTxRx>  {
public:
    HostStat(string ip_string): Hostipv4(ip_string), rxBytes{0}, txBytes{0} {}
    ~HostStat();

    void incTxBytes (uint32_t n);
    void incRxBytes (uint32_t n);

    uint64_t getRxBytes();
    uint64_t getTxBytes();

private:
    uint64_t rxBytes;
    uint64_t txBytes;
    void incTxRxBytes (IncTxRx incTxRx);

    
    
};



#endif //__HOST_STAT__
