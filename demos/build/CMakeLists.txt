cmake_minimum_required(VERSION 3.10)
project(.)

find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
find_library(SDL2_LIBRARY NAME SDL2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O")

#Dynamo library files
file(GLOB_RECURSE INCLUDE "../../src/*.h")
file(GLOB_RECURSE SOURCES "../../src/*.cpp" "../../src/*.c")

#Demo source files
file(GLOB DEMOS "../*.cpp")

# Make Dynamo a shared library
add_library(Dynamo ${SOURCES} ${INCLUDE})

foreach(demo_path ${DEMOS})
    get_filename_component(demo_exe ${demo_path} NAME_WE)
    add_executable(${demo_exe} ${demo_path})

    # Compile with MinGW-w64
    if(WIN32)
        target_link_libraries(${demo_exe} Dynamo mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_net)
    endif()
    
    # Compile on Linux systems
    if(UNIX OR MSVC)
        target_link_libraries(${demo_exe} Dynamo SDL2 SDL2_image SDL2_ttf SDL2_net)
    endif()
endforeach()