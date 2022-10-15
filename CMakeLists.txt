cmake_minimum_required(VERSION 3.10)
project(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O")

add_subdirectory("submodules/fmt")

#Dynamo library files
file(GLOB_RECURSE INCLUDE "./src/*.hpp" "./src/*.h")
file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.c")

# Make Dynamo a shared library
add_library(Dynamo STATIC ${SOURCES} ${INCLUDE})
target_include_directories(Dynamo PUBLIC "./src")

# Link all libraries together
target_link_libraries(Dynamo fmt)