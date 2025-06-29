cmake_minimum_required(VERSION 3.10)
project(BoostUDPServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(boost_1_84_0)

add_executable(main main.cpp)

# Windows soket için
target_link_libraries(main ws2_32)
