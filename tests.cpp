#include "gmock/gmock.h"
#include <iostream>
#include "hostip.h"
#include "hostgroup.h"
#include "group_file_parser.h"
#include "host_stat.h"
#include "group_stat.h"
#include "metric.h"

using namespace testing;
using namespace std;

TEST(ParseHostIp, FromString) {
   Hostipv4 ip {"192.168.20.1"}; 
   ASSERT_THAT (ip.A(), Eq(192));
   ASSERT_THAT (ip.B(), Eq(168));
   ASSERT_THAT (ip.C(), Eq(20));
   ASSERT_THAT (ip.D(), Eq(1));
}

TEST(ParseHostIp, CheckUint32) {
   Hostipv4 ip {"192.168.45.33"}; 
   ASSERT_THAT(ip.to_uint32t(), Eq(0xC0A82D21));
}

TEST(HostGroup, addHost) {
    HostGroup group {"Group A"};
  
    Hostipv4 ip {"192.168.20.1"}; 
    ip.setHostname("host1");
    group.addHost (std::make_shared<Hostipv4> (std::move(ip)));
    ASSERT_THAT (group.size(), Eq(size_t(1))); 
    
    Hostipv4 ip_dup  = ip;
    group.addHost (std::make_shared<Hostipv4> (std::move(ip_dup)));
    ASSERT_THAT (group.size(), Eq(size_t(1))); 

    Hostipv4 ip2{"192.168.20.2"};
    ip2.setHostname("host2");
    group.addHost (std::make_shared<Hostipv4> (std::move(ip2)));
    ASSERT_THAT (group.size(), Eq(size_t(2))); 

    ASSERT_THAT (group.find("host2"), Ne(nullptr));
    
}

TEST(HostGroup, addHostDerivedFromHostipv4) {
    class Hostipv4Derived:public Hostipv4 {
    public:
        Hostipv4Derived (std::string ip_string) : Hostipv4(ip_string) {}
    };
    HostGroup group {"Group A"};
  
    Hostipv4Derived ip {"192.168.20.1"}; 
    ip.setHostname("host1");
    group.addHost (std::make_shared<Hostipv4Derived> (std::move(ip)));
    ASSERT_THAT (group.size(), Eq(size_t(1))); 
    
    Hostipv4Derived ip_dup  = ip;
    group.addHost (std::make_shared<Hostipv4Derived> (std::move(ip_dup)));
    ASSERT_THAT (group.size(), Eq(size_t(1))); 

    Hostipv4Derived ip2{"192.168.20.2"};
    ip2.setHostname("host2");
    group.addHost (std::make_shared<Hostipv4Derived> (std::move(ip2)));
    ASSERT_THAT (group.size(), Eq(size_t(2))); 

    
}

TEST(ParseGroupFile, CheckHostName) {
    class Hostipv4Derived:public Hostipv4 {
    public:
        Hostipv4Derived (std::string ip_string) : Hostipv4(ip_string) {}
    };

    class HostGroupDerived:public HostGroup {
    public:
        HostGroupDerived (std::string name) : HostGroup(name) {}
    };
    string filename = SAMPLE_PCAP_DIR;
    filename += "groups.txt";
    
    auto generateHostipv4Derived = [](std::string ip_string) -> std::shared_ptr<Hostipv4Derived>{
        return std::make_shared<Hostipv4Derived> (ip_string);
        
    };
    auto generateHostGroup = [](std::string name) -> std::shared_ptr<HostGroupDerived>{
        return std::make_shared<HostGroupDerived> (name);
        
    };
    GroupFileParser groupFileParser{filename, generateHostipv4Derived, generateHostGroup};
    auto allGroups = groupFileParser.getAllGroups();

    
    ASSERT_THAT (allGroups.find (std::make_shared<HostGroup>("group1")), Ne(allGroups.end())); 
    ASSERT_THAT (allGroups.find (std::make_shared<HostGroup>("group2")), Ne(allGroups.end())); 
    ASSERT_THAT (allGroups.find (std::make_shared<HostGroup>("group3")), Ne(allGroups.end())); 
    ASSERT_THAT (allGroups.find (std::make_shared<HostGroup>("group4")), Ne(allGroups.end())); 

    auto group2_it = allGroups.find(std::make_shared<HostGroup> ("group2"));
    auto group2 = *group2_it;
    ASSERT_THAT (group2->find ("host5"), Ne (nullptr));
    ASSERT_THAT (group2->find ("host6"), Ne (nullptr));
    ASSERT_THAT (group2->find ("host7"), Ne (nullptr));
    ASSERT_THAT (group2->find ("host8"), Ne (nullptr));

    auto group4_it = allGroups.find(std::make_shared<HostGroup> ("group4"));
    auto group4 = *group4_it;
    ASSERT_THAT (group4->find ("host3"), Ne (nullptr));
    ASSERT_THAT (group4->find ("host4"), Ne (nullptr));
    ASSERT_THAT (group4->find ("host7"), Ne (nullptr));
    ASSERT_THAT (group4->find ("host8"), Ne (nullptr));


    auto mapIpToHost =groupFileParser.getMapIpToHost();
    auto ret = mapIpToHost.find(0x50250925);  // 80.37.9.37
    ASSERT_THAT (ret, Ne(mapIpToHost.end()));
    
    


}


TEST(HostStat, observeUpdate) {
    HostStat host{"10.0.0.15"};
    class UpdateObserver: public AbstractObserver<Metric>{
    public:    

        void onNotified (const Metric& update) override {
            m.add(update);
        }
        Metric m;
    };

    auto updateObserver = std::make_shared<UpdateObserver>();
    host.registerObserver (updateObserver);
    host.add({20,50});
    host.add({20,50});


    ASSERT_THAT(updateObserver->m.txBytes, Eq(uint32_t(40)));
    ASSERT_THAT(updateObserver->m.rxBytes, Eq(uint32_t(100)));


}
TEST(HostStat, observeUpdateWithGroupStat) {
    HostStat host{"10.0.0.15"};
    auto group = std::make_shared<GroupStat>("TestGroup");
    host.registerObserver (group);

    host.add({20,50});
    host.add({20,50});

    auto finalMetric = group->retrieve();
    ASSERT_THAT(finalMetric.txBytes, Eq(uint64_t(40)));
    ASSERT_THAT(finalMetric.rxBytes, Eq(uint64_t(100)));


}


TEST(HostStat, getGroupsOfHost) {

    string filename = SAMPLE_PCAP_DIR;
    filename += "groups.txt";
    
    auto generateHostStat = [](std::string ip_string) -> std::shared_ptr<HostStat>{
        return std::make_shared<HostStat> (ip_string);
        
    };
    auto generateGroupStat = [](std::string name) -> std::shared_ptr<GroupStat>{
        return std::make_shared<GroupStat> (name);
        
    };
    GroupFileParser groupFileParser{filename, generateHostStat, generateGroupStat};

    auto mapIpToHost =groupFileParser.getMapIpToHost();
    auto ret2 = mapIpToHost.find(0x50250925);  // 80.37.9.37
    auto host = static_pointer_cast<HostStat>(ret2->second);
    auto groups = GroupStat::getGroupsOfHost(host);
    set<string> expectedGroupNames  {"group1", "group4"};
    for (auto group: groups) {
        ASSERT_THAT( expectedGroupNames.find(group->getName()), Ne(expectedGroupNames.end()));
        expectedGroupNames.erase(group->getName());
    }
    
    ASSERT_THAT(expectedGroupNames.size(), Eq(set<string>::size_type(0)));
}
int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
