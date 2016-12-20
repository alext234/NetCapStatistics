[![Build Status](https://travis-ci.org/alext234/NetCapStatistics.svg?branch=master)](https://travis-ci.org/alext234/NetCapStatistics)

# NetCapStatistics
A small utility to analyze network capture files (.pcap) and output traffic (tx/rx) by hosts and by groups of hosts.


## Build

Clone the code and update submodules:
```
git clone https://github.com/alext234/NetCapStatistics.git
cd NetCapStatistics
git submodule update --init --recursive
```

Build with cmake
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

```

Run unit tests 
```
./runtests_stat

```


## Usage

We now move to examples directory
```
cd ../examples
../build/ncstat
```
Running ncstat will display help page
```
Analyze and output summary of network captures
Usage:
  ../build/ncstat [OPTION...]

  -h, --help       display this help
  -i, --input arg  pcap file (.cap, .pcap), .lst storing list of pcap files,
                   or network interface (REQUIRED)
  -m, --map arg    mapping file (REQUIRED
```


Mapping file contains the mapping from ip address to hostname and the grouping of hosts.

```
cat groups.txt 
# mappings of ip address to host name
60.234.152.124      host1  
69.44.153.178       host2
80.37.9.37          host3
82.210.155.248      host4
194.109.162.159     host5
202.88.252.50       host6
213.122.214.127     host7
213.202.193.43      host8

# mappings of hosts to groups
[group1]
host1 
host2
host3
host4

[group2]
host5
host6
host7
host8

[group3]
host1
host2
host5
host6

[group4]
host3
host4
host7
host8
```

Running ncstat with all options will give the following output.

```
../build/ncstat -i files.lst -m groups.txt 
start reading from : bt-tf1.pcap
group1:
	Tx: 273 bytes
	Rx: 1,310 bytes
group2:
	Tx: 1,949 bytes
	Rx: 912 bytes
group3:
	Tx: 302 bytes
	Rx: 1,310 bytes
group4:
	Tx: 1,920 bytes
	Rx: 912 bytes
host1 (60.234.152.124):
	Tx: 151 bytes
	Rx: 122 bytes
host2 (69.44.153.178):
	Tx: 0 bytes
	Rx: 822 bytes
host3 (80.37.9.37):
	Tx: 122 bytes
	Rx: 244 bytes
host4 (82.210.155.248):
	Tx: 0 bytes
	Rx: 122 bytes
host5 (194.109.162.159):
	Tx: 0 bytes
	Rx: 122 bytes
host6 (202.88.252.50):
	Tx: 151 bytes
	Rx: 244 bytes
host7 (213.122.214.127):
	Tx: 1,798 bytes
	Rx: 424 bytes
host8 (213.202.193.43):
	Tx: 0 bytes
	Rx: 122 bytes
```


