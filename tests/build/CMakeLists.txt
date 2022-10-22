cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3)

add_subdirectory("../../" "./Dynamo")
add_subdirectory("../../submodules/Catch2" "./Catch2")

# Compile each demo
file(GLOB TESTS "../*.cpp")
foreach(test_path ${TESTS})
    get_filename_component(test ${test_path} NAME_WE)
    add_executable(${test} ${test_path})
    target_link_libraries(${test} Dynamo Catch2WithMain)
endforeach()