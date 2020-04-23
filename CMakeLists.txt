cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

project(ising-model)

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

add_executable(ising-model ${project_SRCS})
