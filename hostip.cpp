#include "hostip.h"


// comparison for set operation      
bool operator< (const Hostipv4 &left, const Hostipv4 &right) {
    return left.isLessThan(right);
}

Hostipv4::Hostipv4 (std::string ip_string) {
    std::regex re {"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)"};
    std::smatch m;
    if (std::regex_match(ip_string, m, re)) {
        a = std::stoi(m[1]);
        b = std::stoi(m[2]);
        c = std::stoi(m[3]);
        d = std::stoi(m[4]);

        hostname = "";
    } else {
        throw  ParseIpException ("invalid IP string");
    }

}
void Hostipv4::setHostname (std::string name) { hostname = name;}
std::string Hostipv4::getHostname () const {return hostname;};
uint32_t Hostipv4::to_uint32t() const { 
    uint32_t ret=d;
    ret|=c<<8;
    ret|=b<<16;
    ret|=a<<24;
    return ret;

} 

bool Hostipv4::isLessThan(const Hostipv4& right) const {
    const Hostipv4& left =*this;

    uint32_t leftIp = left.to_uint32t();
    uint32_t rightIp = right.to_uint32t();       
    if (left.hostname =="" || right.hostname=="") {
        
        return leftIp < rightIp;
    }
    return left.hostname < right.hostname;
}
std::string getIpString (const Hostipv4& host) {
    std::ostringstream ss;
    ss<<int(host.A())<<"."<<int(host.B())<<"."<<int(host.C())<<"."<<int(host.D());
    return ss.str();
}
