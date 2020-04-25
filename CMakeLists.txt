cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

project(ising-model)

# Find all executables
file(GLOB project_SRCS src/*.cpp) #src/*.h

find_package(Python2 COMPONENTS Development NumPy)
include_directories(${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
link_libraries(Python2::Python Python2::NumPy)

find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

add_executable(ising-model ${project_SRCS})
target_link_libraries(ising-model ${OpenCV_LIBRARIES})
