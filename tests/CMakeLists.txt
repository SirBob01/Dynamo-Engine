cmake_minimum_required(VERSION 3.24)
project(.)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_C_STANDARD_REQUIRED on)
set(BUILD_TESTING off)

add_compile_options(-Wall -g -O3 -fPIC)

add_subdirectory("../" "./Dynamo")
add_subdirectory("../submodules/Catch2" "./Catch2")

# Compile each demo
file(GLOB_RECURSE TESTS "./src/*.cpp")
foreach(test_path ${TESTS})
    get_filename_component(test ${test_path} NAME_WE)
    add_executable(${test} ${test_path})
    target_link_libraries(${test} Dynamo Catch2WithMain)
    add_test(NAME ${test} COMMAND ${test})
endforeach()