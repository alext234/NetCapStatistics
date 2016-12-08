#include <iostream>
#include <cxxopts.hpp>
#include <string>

/* 
* command line arguments:
*/
int main (int argc, char* argv[]) 
{
    std::string input; 
    std::string mapfile; 

    cxxopts::Options options(argv[0], "Analyze and output summary of network captures");
    options.add_options()
        ("h,help", "display this help")
        ("i,input", "pcap file, text file storing list of pcap files, or network interface", cxxopts::value<std::string> (input))
        ("m,map", "mapping file", cxxopts::value<std::string>(mapfile))  ;
    options.parse(argc, argv);

    if (options.count("help"))
    {
      std::cout << options.help({""}) << std::endl;
      exit(0);
    }
}
