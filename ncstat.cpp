#include <iostream>
#include <cxxopts.hpp>
#include <string>
#include <fstream>
#include "packet_stat.h"
#include "cpppcap.h"
#include "group_file_parser.h"
#include "group_stat.h"
#include "summary.h"


using namespace std;
using namespace Pcap;

void processPcapList (shared_ptr<PacketStat>& packetStat, const vector<string>& pcapList) {


    for (const auto& pcapFile: pcapList) {
        cout <<"start reading from : "<<pcapFile<<endl;

        auto dev = openOffline(pcapFile);
        dev->registerObserver(packetStat);
        dev->loop();        
       
    }


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
    auto generateHostGroup = [](std::string name) -> std::shared_ptr<GroupStat>{
        return std::make_shared<GroupStat> (name);
        
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


        
        Summary summary (packetStat, groupFileParser.getAllGroups(), mapIpToHost);
        processPcapList (packetStat, pcapList);
        
        
        cout << summary.summaryString()<<endl;
        
    }
}
