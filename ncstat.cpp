#include <iostream>
#include <cxxopts.hpp>
#include <string>
#include <fstream>
#include "packet_stat.h"
#include "cpppcap.h"
#include "group_file_parser.h"

using namespace std;
using namespace Pcap;

void printIp (uint32_t ip) {
        cout << ((ip>>24)&0xff) <<".";
        cout << ((ip>>16)&0xff) <<".";
        cout << ((ip>>8)&0xff) <<".";
        cout << ((ip)&0xff) <<endl;
}
void processPcapList (shared_ptr<PacketStat>& packetStat, const vector<string>& pcapList) {


    for (const auto& pcapFile: pcapList) {
        auto dev = openOffline(pcapFile);
        dev->registerObserver(packetStat);
        dev->loop();        
       
    }


}


void printStats (shared_ptr<PacketStat>& packetStat,GroupFileParser& groupFileParser  ) {
    auto listOfUnmappedIps = packetStat->getListOfUnmappedIps();
    if (listOfUnmappedIps.size() >0) {
        cout<<"these IP's are not mapped to hostname :" <<endl;
        for (auto ip: listOfUnmappedIps) { printIp(ip);    }
    }

    cout<<dec;
    cout.imbue(std::locale("")); //  // imbue global locale ; for comma separated numbers

    auto allGroups = groupFileParser.getAllGroups();
    for (auto group: allGroups) { // NOTE: this is not so efficient and only offline, not good for realtime stats update
        auto allHostsInGroup = group->getHosts();

        uint64_t groupTx=0, groupRx=0;
        for (auto host: allHostsInGroup) {
            groupTx +=static_pointer_cast<HostStat>(host)->getTxBytes();
            groupRx +=static_pointer_cast<HostStat>(host)->getRxBytes();
        }
        cout << group->getName() <<":"<<endl;
        cout <<'\t'<<"Tx: "<<groupTx<<" bytes"<<endl;
        cout <<'\t'<<"Rx: "<<groupRx<<" bytes"<<endl;
    }
   
    
    auto allMappedHosts =groupFileParser.getAllhostGroup()->getHosts();
    uint64_t allMappedHostsTx=0;
    uint64_t allMappedHostsRx=0;
    for (auto host: allMappedHosts) {
        
        auto hostC = static_pointer_cast<HostStat>(host);
        cout << hostC->getIpString() <<" "<<hostC->getHostname() <<":"<<endl;
        allMappedHostsTx+= hostC->getTxBytes();
        allMappedHostsRx+= hostC->getRxBytes();
        cout <<'\t'<<"Tx: "<<hostC->getTxBytes()<<" bytes"<<endl;
        cout <<'\t'<<"Rx: "<<hostC->getRxBytes()<<" bytes"<<endl;
   }

   cout <<"All mapped hosts Tx: "<<allMappedHostsTx<<" bytes"<<endl;
   cout <<"All mapped hosts Rx: "<<allMappedHostsRx<<" bytes"<<endl;


        
}
/* 
* command line arguments:
*/
int main (int argc, char* argv[]) 
{
    string input; 
    string mapfile; 

    cxxopts::Options options(argv[0], "Analyze and output summary of network captures");
    options.add_options()
        ("h,help", "display this help");

    options.add_option ("","i", "input", "pcap file (.cap, .pcap), .lst storing list of pcap files, or network interface", cxxopts::value<string> (input),"",  true);
    options.add_option ("", "m", "map", "mapping file", cxxopts::value<string>(mapfile), "", true)  ;
    options.parse(argc, argv);

    if (options.count("help")||input=="" || mapfile=="")
    {
      cout << options.help({""}) << endl;
      exit(0);
    }   
    auto generateHostStat = [](std::string ip_string) -> std::shared_ptr<HostStat>{
        return std::make_shared<HostStat> (ip_string);
        
    };    
    auto generateHostGroup = [](std::string name) -> std::shared_ptr<HostGroup>{
        return std::make_shared<HostGroup> (name);
        
    };
    // parse mapping file
    GroupFileParser groupFileParser{mapfile, generateHostStat, generateHostGroup}; 
    auto mapIpToHost  = groupFileParser.getMapIpToHost();
    
    auto packetStat = make_shared<PacketStat>(mapIpToHost);
    // check file extension
    auto dotPos = input.find_last_of('.');
    if (dotPos ==  string::npos) {
        // TODO: open live network interface
    } else {
        vector<string> pcapList;
        auto ext = input.substr(dotPos+1);
        if (ext=="lst") {
            ifstream ifs(input,ifstream::in);
            string line;
            while(getline(ifs, line).good()) {
                pcapList.push_back(line);
            }
            ifs.close();

        } else if (ext=="pcap" || ext=="cap" ) {
            pcapList.push_back(input);
        } else {
            cout << "invalid inut file extension " << endl;
            exit(1);
        }


        cout <<"start reading from : ";
        for (const auto& f: pcapList) { cout << '\t' << f; }
        cout << endl;

        processPcapList (packetStat, pcapList);

        printStats (packetStat, groupFileParser);
        
    }
}
