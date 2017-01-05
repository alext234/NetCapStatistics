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
    Hostipv4 (std::string ip_string);
    virtual ~Hostipv4() {}
   
    void setHostname (std::string name);
    std::string getHostname () const ;

    uint8_t A() const {return a;};
    uint8_t B() const {return b;};
    uint8_t C() const {return c;};
    uint8_t D() const {return d;};
    uint32_t to_uint32t() const ;

    bool isLessThan(const Hostipv4& right) const;


private:    
    uint8_t a=0,b=0,c=0,d=0;   
    std::string hostname;

 
};
bool operator< (const Hostipv4 &left, const Hostipv4 &right);
std::string getIpString (const Hostipv4& host) ;
#endif //__HOST_IP__
