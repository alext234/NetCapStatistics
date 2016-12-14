#ifndef __HOST_IP__
#define __HOST_IP__
#include <cstdint>
#include <string>
#include <regex>

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

            hostname = ip_string;
        } else {
            throw  ParseIpException ("invalid IP string");
        }

    }
   
    void setHostname (std::string name) { hostname = name;}
    std::string getHostname () {return hostname;};
    uint8_t A() {return a;};
    uint8_t B() {return b;};
    uint8_t C() {return c;};
    uint8_t D() {return d;};

    // comparison for set operation      
    friend bool operator< (const Hostipv4 &left, const Hostipv4 &right) {
        return left.hostname< right.hostname;
    }

private:    
    uint8_t a,b,c,d;   
    std::string hostname;
};
#endif //__HOST_IP__