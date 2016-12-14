#include "gmock/gmock.h"
#include <iostream>
#include "hostip.h"

using namespace testing;

TEST(ParseHostIp, FromString) {
   Hostipv4 ip {"192.168.20.1"}; 
   ASSERT_THAT (ip.a, Eq(192));
   ASSERT_THAT (ip.b, Eq(168));
   ASSERT_THAT (ip.c, Eq(20));
   ASSERT_THAT (ip.d, Eq(1));
}

TEST(ParseGroupFile, CheckHostAlias) {
    // TODO: given a group file with mapping ip -> hostname, check if the parsing is correct
}

int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
