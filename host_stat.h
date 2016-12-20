#ifndef __HOST_STAT__
#define __HOST_STAT__
#include "hostip.h"

using namespace std;

class HostStat: public Hostipv4  {
public:
    HostStat(string ip_string): Hostipv4(ip_string), rxBytes{0}, txBytes{0} {}

    void incTxBytes (uint32_t n);
    void incRxBytes (uint32_t n);

    uint64_t getRxBytes();
    uint64_t getTxBytes();

private:
    uint64_t rxBytes;
    uint64_t txBytes;
    
};



#endif //__HOST_STAT__
