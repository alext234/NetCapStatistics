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

    void add (Metric a);
    const Metric & retrieve();
private:
    Metric m;

    
    
};



#endif //__HOST_STAT__
