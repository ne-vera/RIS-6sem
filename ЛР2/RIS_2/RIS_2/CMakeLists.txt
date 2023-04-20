cmake_minimum_required(VERSION 3.21)
project(Lab_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -static")

add_executable(Lab_2 main.cpp Error.cpp Error.h SectionControlUnit.h SectionControlUnit.cpp)
target_link_libraries(Lab_2 ws2_32)