cmake_minimum_required(VERSION 3.10)
project(udp_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(udp_server main.cpp)

target_include_directories(udp_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(udp_server PRIVATE ${Boost_LIBRARIES})
