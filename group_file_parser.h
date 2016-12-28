#ifndef __GROUP_FILE_PARSER__
#define __GROUP_FILE_PARSER__
#include "hostgroup.h"
#include "hostip.h"
#include <fstream>
#include <string>
#include <memory>
#include <type_traits>
#include <map>

class GroupFileParserException: public std::exception {
public:
    GroupFileParserException (std::string r): _reason(r) {}
    const char* what() const noexcept {
        return _reason.c_str();
    }
private:
    std::string _reason;
};




class GroupFileParser {
public:      
    using Hostipv4FactoryFunction= std::function<std::shared_ptr<Hostipv4>(std::string)>; 
    using HostGroupFactoryFunction= std::function<std::shared_ptr<HostGroup>(std::string)>; 
    using ptr_HostGroup = std::shared_ptr<HostGroup>;
    using SetPtrHostGroup = std::set<ptr_HostGroup, shared_ptr_less<HostGroup>>;
    using MapIpToHost = std::map<uint32_t, std::shared_ptr<Hostipv4>> ;
    GroupFileParser(std::string filename, Hostipv4FactoryFunction  hostipv4Factory, HostGroupFactoryFunction hostgroupFactory)
    : hostipv4Factory{hostipv4Factory}, hostgroupFactory{hostgroupFactory} {
        parseFile(filename);
    }

    const auto& getAllGroups() { return allGroups;};
    const auto& getMapIpToHost() { return mapIpToHost;} 
private:
    Hostipv4FactoryFunction hostipv4Factory;
    HostGroupFactoryFunction hostgroupFactory;
    void parseFile(std::string filename);
    SetPtrHostGroup allGroups;
    MapIpToHost mapIpToHost; 
    
};
#endif // __GROUP_FILE_PARSER__

