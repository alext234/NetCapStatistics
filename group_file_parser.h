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


struct shared_ptr_host_group_compare {
    bool operator() (const std::shared_ptr<HostGroup>& lhs, const std::shared_ptr<HostGroup>& rhs) const{
        return lhs->getName() < rhs->getName();
    }
};

template <typename T> // T should be Hostipv4 or its derived class
class GroupFileParser {
public:
    GroupFileParser(std::string filename) : allHosts{new HostGroup{"GlobalGroup"}} {
        static_assert (  std::is_base_of<Hostipv4, T>::value, "T should be Hostipv4 or its derived class");
        parseFile(filename);
    }
    const auto getAllGroups() { return allGroups;};

private:
    void parseFile(std::string filename);
    using ptr_HostGroup = std::shared_ptr<HostGroup>;
    ptr_HostGroup allHosts;
    std::set<ptr_HostGroup, shared_ptr_host_group_compare> allGroups;
    
    
};

//// class implementation
template <typename T>
void GroupFileParser<T>::parseFile(std::string filename) {
    std::ifstream ifs(filename);
    std::string line;
    ptr_HostGroup currentGroup=nullptr;
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
                allHosts->addHost (hostIp);

            } catch (const ParseIpException& ex) {
                throw;
            }
        } else 
        if (first.length()>0 && second.length()==0) {
            if (first[0] =='[') {
                auto closeFound = first.find(']', 1);
                if (closeFound==std::string::npos) { // not found
                    throw  GroupFileParserException("invalid syntax at '"+line+"'");
                }
                auto groupName= first.substr(1,closeFound-1);
                auto newGroup = std::make_shared<HostGroup> (groupName);
                
                auto r = allGroups.insert(newGroup);
                if (r.second==false) {
                    newGroup = *r.first; // access to the same group
                }
                currentGroup = newGroup;

            } else { 
                // should be hostname. check if exist
                auto host =  allHosts->find(first);
                if (host  ==nullptr) {
                    throw GroupFileParserException("hostname not defined at '"+line+"'");
                }
                if (currentGroup == nullptr) {
                    throw GroupFileParserException("hostname without a group at '"+line+"'");
                }
                currentGroup->addHost(host);
                
            }

        }


        
    }
    ifs.close();

}


#endif // __GROUP_FILE_PARSER__
