#ifndef __HOST_IP__
#define __HOST_IP__
#include <cstdint>
#include <string>
#include <regex>
#include <sstream>
#include <set>
#include <memory>
#include <tuple>


#include <iostream>
class ParseIpException: public std::exception {
public:
    ParseIpException(std::string r) : _reason(r) {}
    const char *what () const noexcept {    
        return _reason.c_str();
    }
private:
    std::string _reason;
};

class Hostipv4 {
public:
    Hostipv4 (std::string ip_string) {
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
    virtual ~Hostipv4() {}
   
    void setHostname (std::string name) { hostname = name;}
    std::string getHostname () const {return hostname;};
    std::string getIpString () const {
        std::ostringstream ss;
        ss<<int(a)<<"."<<int(b)<<"."<<int(c)<<"."<<int(d);
        return ss.str();
    }
    uint8_t A() {return a;};
    uint8_t B() {return b;};
    uint8_t C() {return c;};
    uint8_t D() {return d;};
    uint32_t to_uint32t() const { 
        uint32_t ret=d;
        ret|=c<<8;
        ret|=b<<16;
        ret|=a<<24;
        return ret;

    } 
    // comparison for set operation      
    friend bool operator< (const Hostipv4 &left, const Hostipv4 &right) {
        uint32_t leftIp = left.to_uint32t();
        uint32_t rightIp = right.to_uint32t();       
        if (left.hostname =="" || right.hostname=="") {
            
            return leftIp < rightIp;
        }
        return left.hostname < right.hostname;
    }

private:    
    uint8_t a=0,b=0,c=0,d=0;   
    std::string hostname;

 
};
#endif //__HOST_IP__
