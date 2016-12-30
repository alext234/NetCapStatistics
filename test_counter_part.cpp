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
    
    
    CounterPartList<HostStat> cplist; 
    auto A = make_shared<HostStat>("10.0.0.1");
    auto B = make_shared<HostStat>("10.0.0.2");
    cplist.add (A, {TxBytes(2),RxBytes(5)}); 
    cplist.add (B, {TxBytes(4),RxBytes(6)}); 
    cplist.add (A, {TxBytes(3),RxBytes(7)}); 
    cplist.add (B, {TxBytes(5),RxBytes(8)}); 

    Metric totalA = cplist.retrieve(A);
    Metric totalB = cplist.retrieve(make_shared<HostStat>("10.0.0.2"));

    ASSERT_THAT (totalA.txBytes, Eq(uint64_t(5)));
    ASSERT_THAT (totalA.rxBytes, Eq(uint64_t(12)));
    ASSERT_THAT (totalB.txBytes, Eq(uint64_t(9)));
    ASSERT_THAT (totalB.rxBytes, Eq(uint64_t(14)));
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
    auto peerList = host1->getPeerList();
    Metric pm;
    // retrieve stats with peer, should not throw exception
    //  0x50250925   80.37.9.37
    pm = peerList.retrieve(static_pointer_cast<HostStat>(mapIpToHost.find(0x50250925)->second)); 
    ASSERT_THAT (pm.txBytes, Eq(uint64_t(244)));
    ASSERT_THAT (pm.rxBytes, Eq(uint64_t(122)));


    //  0x52D29BF8 82.210.155.248
    pm = peerList.retrieve(static_pointer_cast<HostStat>(mapIpToHost.find(0x52D29BF8)->second)); 
    //  0x452C99B2 69.44.153.178
    pm = peerList.retrieve(static_pointer_cast<HostStat>(mapIpToHost.find(0x452C99B2)->second)); 
    //  0xC26DA29F  194.109.162.159 
    pm = peerList.retrieve(static_pointer_cast<HostStat>(mapIpToHost.find(0xC26DA29F)->second)); 

    
    //  0xC26DA29F  194.109.162.159 
    pm = peerList.retrieve(make_shared<HostStat>("194.109.162.159")); 


}
TEST(CounterPartList, checkListOfGroupPeers) {
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


    // get a specific group
    auto allGroups = groupFileParser.getAllGroups();
    auto group5_it = allGroups.find (make_shared<GroupStat>("group5"));
    auto group5 = static_pointer_cast<GroupStat> (*group5_it);

    // get list of peers
    auto peerList = group5->getPeerList();


    Metric pm;
    // should not have exception
    pm = peerList.retrieve(make_shared<GroupStat>("group6"));
    // verify tx bytes and rx bytes
    ASSERT_THAT (pm.txBytes, Eq(uint64_t(151)));  // i.e. send to this peer 151 bytes
    ASSERT_THAT (pm.rxBytes, Eq(uint64_t(122))); 


    auto group1_it = allGroups.find (make_shared<GroupStat>("group1"));
    auto group1 = static_pointer_cast<GroupStat> (*group1_it);

    // get list of peers
    peerList = group1->getPeerList();


    // should not have exception
    pm = peerList.retrieve(make_shared<GroupStat>("group2"));
    // verify tx bytes and rx bytes
    ASSERT_THAT (pm.txBytes, Eq(uint64_t(273))); 
    ASSERT_THAT (pm.rxBytes, Eq(uint64_t(1310))); 


}
