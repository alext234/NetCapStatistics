#ifndef __HOST_STAT__
#define __HOST_STAT__
#include "hostip.h"
#include "cpp_observer.h"
#include "metric.h"
#include <memory>
#include "counter_part.h"
using namespace std;


struct HostStatNotifiedData {
   Metric m; 
   // TODO add host and peer
};

class HostStat: public Hostipv4, public Observable<HostStatNotifiedData>, public std::enable_shared_from_this<HostStat>  {
public:
    HostStat(string ip_string): Hostipv4(ip_string) {}
    ~HostStat();

    void add(std::shared_ptr<HostStat> peer, Metric a);
    void add (Metric a);
    const Metric & retrieve();
    const CounterPartList<HostStat>& getPeerList();
private:
    Metric m;
    CounterPartList<HostStat> peerList;
   
    
    
};



#endif //__HOST_STAT__
