#include "gmock/gmock.h"
#include "counter_part.h"
#include "hostip.h"

using namespace testing;
using namespace std;

TEST(CounterPartList, checkCountersUpdate) {
    
    
    CounterPartList<Hostipv4> cplist; 
    auto A = make_shared<Hostipv4>("10.0.0.1");
    auto B = make_shared<Hostipv4>("10.0.0.2");
    cplist.add (A, {2,2}); 
    cplist.add (B, {3,3}); 
    cplist.add (A, {2,2}); 
    cplist.add (B, {3,3}); 

    Metric totalA = cplist.retrieve(A);
    Metric totalB = cplist.retrieve(B);

    ASSERT_THAT (totalA.txBytes, Eq(uint64_t(4)));
    ASSERT_THAT (totalA.rxBytes, Eq(uint64_t(4)));
    ASSERT_THAT (totalB.txBytes, Eq(uint64_t(6)));
    ASSERT_THAT (totalB.rxBytes, Eq(uint64_t(6)));
}


