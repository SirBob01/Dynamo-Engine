cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3)

add_subdirectory("../" "./Dynamo")

# Compile each demo
file(GLOB DEMOS "./*.cpp")
foreach(demo_path ${DEMOS})
    get_filename_component(demo ${demo_path} NAME_WE)
    add_executable(${demo} ${demo_path})
    target_link_libraries(${demo} Dynamo)
endforeach()