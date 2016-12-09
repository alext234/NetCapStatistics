cmake_minimum_required(VERSION 2.8)

project(NetCapStatistics)

add_subdirectory(LibPcap)
if (NOT TARGET gtest) # googletest library may have been added by other subdirectory
    add_subdirectory(googletest)
endif() 

set(CPPOBSERVER ${CMAKE_CURRENT_SOURCE_DIR}/cppobserver)  
set(CPP_LIBPCAP ${CMAKE_CURRENT_SOURCE_DIR}/LibPcap)  
set(CXXOPTS ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts)  

set(GOOGLETEST ${CMAKE_CURRENT_SOURCE_DIR}/googletest/)  
include_directories(${GOOGLETEST}/googlemock/include ${GOOGLETEST}/googletest/include)
link_directories(${GOOGLETEST}/build/googlemock ${GOOGLETEST}/build/googlemock/gtest)
include_directories(${CPPOBSERVER})
include_directories(${CPP_LIBPCAP})
include_directories(${CXXOPTS}/include)

# set build type: cmake -DCMAKE_BUILD_TYPE=Debug  ; to see all options of AddressSanitizer, set the environment variable ASAN_OPTIONS=help=1
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -std=c++1y -Wall -Wextra -Wno-unused-parameter -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -g -std=c++1y -Wall -Wextra -Wno-unused-parameter")


add_executable(runtests_stat  tests.cpp)
set(SAMPLE_PCAP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
set_target_properties(runtests_stat PROPERTIES COMPILE_FLAGS -D'SAMPLE_PCAP_DIR="${SAMPLE_PCAP_DIR}"')
target_link_libraries(runtests_stat gmock)
target_link_libraries(runtests_stat gtest)
target_link_libraries(runtests_stat pthread)
target_link_libraries(runtests_stat cpppcap)
target_link_libraries(runtests_stat pcap)


add_executable(ncstat ncstat.cpp)
add_dependencies(ncstat runtests_stat) # test binaries should be ok before building the actual app
target_link_libraries(ncstat pcap)
