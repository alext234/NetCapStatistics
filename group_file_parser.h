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
    bool operator() (const std::shared_ptr<HostGroup>& lhs, const std::shared_ptr<HostGroup>& rhs) const;
};

class GroupFileParser {
public:      
    using Hostipv4FactoryFunction= std::function<std::shared_ptr<Hostipv4>(std::string)>; 
    using HostGroupFactoryFunction= std::function<std::shared_ptr<HostGroup>(std::string)>; 

    GroupFileParser(std::string filename, Hostipv4FactoryFunction  hostipv4Factory, HostGroupFactoryFunction hostgroupFactory)
    : hostipv4Factory{hostipv4Factory}, hostgroupFactory{hostgroupFactory},allHosts{new HostGroup{"GlobalGroup"}} {
        parseFile(filename);
    }

    GroupFileParser(std::string filename) : allHosts{new HostGroup{"GlobalGroup"}} {
    }
    const auto getAllGroups() { return allGroups;};
    const auto getAllhostGroup() { return allHosts;};
    const auto& getMapIpToHost() { return mapIpToHost;} 
private:
    Hostipv4FactoryFunction hostipv4Factory;
    HostGroupFactoryFunction hostgroupFactory;
    void parseFile(std::string filename);
    using ptr_HostGroup = std::shared_ptr<HostGroup>;
    ptr_HostGroup allHosts;
    std::set<ptr_HostGroup, shared_ptr_host_group_compare> allGroups;
    std::map<uint32_t, std::shared_ptr<Hostipv4>> mapIpToHost; 
    
};
#endif // __GROUP_FILE_PARSER__

