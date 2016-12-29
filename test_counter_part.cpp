#include "gmock/gmock.h"
#include "counter_part.h"
#include "hostip.h"
#include "packet_stat.h"
#include "group_file_parser.h"
#include "group_stat.h"
#include "host_stat.h"
#include "packet_stat.h"

using namespace testing;
using namespace std;

TEST(CounterPartList, checkCountersUpdate) {
    
    
    CounterPartList<Hostipv4> cplist; 
    auto A = make_shared<Hostipv4>("10.0.0.1");
    auto B = make_shared<Hostipv4>("10.0.0.2");
    cplist.add (A, {TxBytes(2),RxBytes(2)}); 
    cplist.add (B, {TxBytes(3),RxBytes(3)}); 
    cplist.add (A, {TxBytes(2),RxBytes(2)}); 
    cplist.add (B, {TxBytes(3),RxBytes(3)}); 

    Metric totalA = cplist.retrieve(A);
    Metric totalB = cplist.retrieve(B);

    ASSERT_THAT (totalA.txBytes, Eq(uint64_t(4)));
    ASSERT_THAT (totalA.rxBytes, Eq(uint64_t(4)));
    ASSERT_THAT (totalB.txBytes, Eq(uint64_t(6)));
    ASSERT_THAT (totalB.rxBytes, Eq(uint64_t(6)));
}

TEST(CounterPartList, checkListOfHostPeers) {
    string dir = SAMPLE_PCAP_DIR;
    string mapfile =dir+ "groups.txt";
    string pcapfile =dir+ "bt-tf1.pcap";

    auto generateHostStat = [](std::string ip_string) -> std::shared_ptr<HostStat>{
        return std::make_shared<HostStat> (ip_string);
        
    };    
    auto generateHostGroup = [](std::string name) -> std::shared_ptr<GroupStat>{
        return std::make_shared<GroupStat> (name);
        
    };
    // parse mapping file
    GroupFileParser groupFileParser{mapfile, generateHostStat, generateHostGroup}; 
    auto mapIpToHost  = groupFileParser.getMapIpToHost();
    
    auto packetStat = make_shared<PacketStat>(mapIpToHost);
    
    auto dev = openOffline(pcapfile);
    dev->registerObserver(packetStat);
    dev->loop();        

    // get a specific host
    auto ret1 = mapIpToHost.find(0xD57AD67F);  // 213.122.214.127
    auto host1 = static_pointer_cast<HostStat>(ret1->second);
    // TODO:
    //auto listPeers = host1.getListOfPeers();
    // verify that list of peers should include 
    //  0x50250925   80.37.9.37
    //  0x52D29BF8 82.210.155.248
    //  0x452C99B2 69.44.153.178
    //  0xC26DA29F  194.109.162.159 


}
