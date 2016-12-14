#ifndef __GROUP_FILE_PARSER__
#define __GROUP_FILE_PARSER__
#include "hostgroup.h"
#include "hostip.h"
#include <fstream>
#include <string>
#include <memory>

class ParseGroupFileException: public std::exception {
public:
    ParseGroupFileException (std::string r): _reason(r) {}
    const char* what() const noexcept {
        return _reason.c_str();
    }
private:
    std::string _reason;
};

class GroupFileParser {
public:
    GroupFileParser(std::string filename) {
        parseFile(filename);
    }

private:
    void parseFile(std::string filename);
    std::map<std::string,std::shared_ptr<Hostipv4>> listOfHostIp;
    
};


//// class implementation

void GroupFileParser::parseFile(std::string filename) {
    std::ifstream ifs(filename);
    std::string line;
    while(std::getline(ifs, line).good()) {
        std::istringstream iss(line);
        std::string first, second;
        iss >> first>> second;
       
        if (first.length()>0 && first[0]=='#') {
            continue; // comment line
        } else 
        if (first.length()>0 && second.length()>0) {
            try {
                auto hostIp = std::make_shared<Hostipv4> (first);
                hostIp -> setHostname (second);
                listOfHostIp[first] = hostIp;                

            } catch (const ParseIpException& ex) {
                throw;
            }
        } else 
        if (first.length()>0 && second.length()==0) {
            // TODO: handle [] and hostname 
            // TODO: note the case of comment

            //throw ParseGroupFileException if needed
        }


        
    }
    ifs.close();

}


#endif // __GROUP_FILE_PARSER__
