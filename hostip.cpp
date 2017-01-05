#include "hostip.h"


// comparison for set operation      
bool operator< (const Hostipv4 &left, const Hostipv4 &right) {
    return left.isLessThan(right);
}
