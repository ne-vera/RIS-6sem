cmake_minimum_required(VERSION 3.21)
project(Lav_2_client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")
add_executable(Lav_2_client main.cpp Error.h Error.cpp Time.cpp Time.h SectionControlUnit.cpp SectionControlUnit.h)
target_link_libraries(Lav_2_client ws2_32)
