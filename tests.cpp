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


TEST(HostGroup, addHost) {
    HostGroup group {"Group A"};
  
    Hostipv4 ip {"192.168.20.1"}; 
    ip.setHostname("host1");
    group.addHost (ip);
    ASSERT_THAT (group.size(), Eq(size_t(1))); 
    
    Hostipv4 ip_dup = ip;
    group.addHost (ip_dup);  // check if it can detect duplicate
    ASSERT_THAT (group.size(), Eq(size_t(1))); 

    Hostipv4 ip2{"192.168.20.2"};
    ip2.setHostname("host2");
    group.addHost (ip2);
    ASSERT_THAT (group.size(), Eq(size_t(2))); 

    
}

TEST(ParseGroupFile, CheckHostName) {
    string filename = SAMPLE_PCAP_DIR;
    filename += "groups.txt";

    GroupFileParser groupFileParser{filename};
    //auto listOfHostIp = groupFileParser.getListOfHostIp();
}

int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
