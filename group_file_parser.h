#ifndef __GROUP_FILE_PARSER__
#define __GROUP_FILE_PARSER__
#include "hostgroup.h"
#include "hostip.h"
#include <fstream>
#include <string>
#include <memory>
#include <type_traits>

class GroupFileParserException: public std::exception {
public:
    GroupFileParserException (std::string r): _reason(r) {}
    const char* what() const noexcept {
        return _reason.c_str();
    }
private:
    std::string _reason;
};

template <typename T> // T should be Hostipv4 or its derived class
class GroupFileParser {
public:
    GroupFileParser(std::string filename) {
        static_assert (  std::is_base_of<Hostipv4, T>::value, "T should be Hostipv4 or its derived class");
        parseFile(filename);
    }

private:
    void parseFile(std::string filename);
    std::map<std::string,std::shared_ptr<T>> listOfHostIp;
    
};


//// class implementation
template <typename T>
void GroupFileParser<T>::parseFile(std::string filename) {
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
                auto hostIp = std::make_shared<T> (first);
                hostIp -> setHostname (second);
                listOfHostIp[first] = hostIp;                

            } catch (const ParseIpException& ex) {
                throw;
            }
        } else 
        if (first.length()>0 && second.length()==0) {
            // TODO: handle [] and hostname 
            if (first[0] =='[') {
                auto closeFound = first.find(']', 1);
                if (closeFound==std::string::npos) { // not found
                    throw  GroupFileParserException("invalid syntax at '"+line+"'");
                }
                auto groupName= first.substr(1,closeFound-1);
                // TODO: create group and add to group list; track the current group
            }
            // TODO: note the case of comment

            //throw ParseGroupFileException if needed
        }


        
    }
    ifs.close();

}


#endif // __GROUP_FILE_PARSER__
