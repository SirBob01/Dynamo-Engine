cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3 -fPIC)

find_package(Vulkan REQUIRED glslangValidator)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory("submodules/fmt")
add_subdirectory("submodules/glfw")
add_subdirectory("submodules/portaudio")
add_subdirectory("submodules/tinyobjloader")

set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "SPIRV-Reflect executable")
set(SPIRV_REFLECT_EXAMPLES OFF CACHE BOOL "SPIRV-Reflect examples")
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "SPIRV-Reflect static library")
add_subdirectory("submodules/spirv-reflect")

set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTING OFF CACHE BOOL "Libsndfile testing")
set(BUILD_PROGRAMS OFF CACHE BOOL "Libsndfile programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Libsndfile exapmles")
add_subdirectory("submodules/libsndfile")


# Dynamo library files
file(GLOB_RECURSE INCLUDE "./src/*.hpp" "./src/*.h")
file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.c")

# Make Dynamo a shared library
add_library(Dynamo SHARED ${SOURCES} ${INCLUDE} ${STB_SOURCES})
target_include_directories(Dynamo PUBLIC "./src" "./submodules/stb")

# Link all libraries together
target_link_libraries(Dynamo fmt glfw Vulkan::Vulkan PortAudio sndfile tinyobjloader spirv-reflect-static)
