#include <iostream>
#include <cxxopts.hpp>
#include <string>
#include <fstream>
#include "packet_stat.h"
#include "cpppcap.h"

using namespace std;
using namespace Pcap;

shared_ptr<PacketStat> processPcapList (const vector<string>& pcapList) {

    auto packetStat = make_shared<PacketStat>();

    for (const auto& pcapFile: pcapList) {
        auto dev = openOffline(pcapFile);
        dev->registerObserver(packetStat);
        dev->loop();        
       
    }

    return packetStat;
    
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
        auto stat = processPcapList (pcapList);
        stat -> print();

        
    }
}
