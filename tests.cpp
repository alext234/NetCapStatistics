#include "gmock/gmock.h"
#include <iostream>
#include "hostip.h"
#include "hostgroup.h"
#include "group_file_parser.h"

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
    string filename = SAMPLE_PCAP_DIR;
    filename += "groups.txt";

    GroupFileParser<Hostipv4Derived> groupFileParser{filename};
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
}

int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
