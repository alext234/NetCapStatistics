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

struct Hostipv4 {
    Hostipv4 (std::string ip_string) {
        std::regex re {"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)"};
        std::smatch m;
        if (std::regex_match(ip_string, m, re)) {
            a = std::stoi(m[1]);
            b = std::stoi(m[2]);
            c = std::stoi(m[3]);
            d = std::stoi(m[4]);


        } else {
            throw  ParseIpException ("invalid IP string");
        }

    }
    uint8_t a,b,c,d;   
};
#endif //__HOST_IP__
