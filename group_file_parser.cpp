#include "group_file_parser.h"


void GroupFileParser::parseFile(std::string filename) {
    std::ifstream ifs(filename);
    std::string line;
    ptr_HostGroup currentGroup=nullptr;
    auto allHosts = std::make_shared<HostGroup> ("AllHosts");
    while(std::getline(ifs, line).good()) {
        std::istringstream iss(line);
        std::string first, second;
        iss >> first>> second;
       
        if (first.length()>0 && first[0]=='#') {
            continue; // comment line
        } else 
        if (first.length()>0 && second.length()>0) {
            try {
                std::shared_ptr<Hostipv4> hostIp = hostipv4Factory(first);
                hostIp -> setHostname (second);
                allHosts->addHost (hostIp);
                mapIpToHost[hostIp->to_uint32t()] = hostIp;

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
                auto newGroup = hostgroupFactory(groupName);
                
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


