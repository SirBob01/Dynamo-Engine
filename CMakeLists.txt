cmake_minimum_required(VERSION 3.24)
project(.)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)

add_compile_options(-Wall -g -O3)

find_package(Vulkan REQUIRED glslangValidator)

add_subdirectory("submodules/fmt")
add_subdirectory("submodules/glfw")
add_subdirectory("submodules/portaudio")

# Dynamo library files
file(GLOB_RECURSE INCLUDE "./src/*.hpp" "./src/*.h")
file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.c")
file(GLOB STB_SOURCES "./submodules/stb/*.c")

# Make Dynamo a shared library
add_library(Dynamo STATIC ${SOURCES} ${INCLUDE} ${STB_SOURCES})
target_include_directories(Dynamo PUBLIC "./src" "./submodules/stb")

# Link all libraries together
target_link_libraries(Dynamo fmt glfw Vulkan::Vulkan PortAudio)
