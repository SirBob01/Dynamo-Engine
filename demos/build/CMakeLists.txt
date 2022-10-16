cmake_minimum_required(VERSION 3.24)
project(.)

add_subdirectory("../.." "./Dynamo")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O3")

# Compile each demo
file(GLOB DEMOS "../*.cpp")
foreach(demo_path ${DEMOS})
    get_filename_component(demo_exe ${demo_path} NAME_WE)
    add_executable(${demo_exe} ${demo_path})

    # Compile with MinGW-w64
    if(WIN32)
        target_link_libraries(${demo_exe} Dynamo mingw32)
    endif()

    # Compile on Unix systems
    if(UNIX OR MSVC)
        target_link_libraries(${demo_exe} Dynamo)
    endif()
endforeach()