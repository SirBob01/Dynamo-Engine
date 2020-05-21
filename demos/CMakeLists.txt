cmake_minimum_required(VERSION 3.10)
project(.)
 
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -O -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net -lvorbis -lvorbisfile")

#Bring the headers, such as Student.h into the project
file(GLOB_RECURSE INCLUDE "../src/*.h")

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "../src/*.cpp")
 
add_executable(./bin/audio audio.cpp ${SOURCES} ${INCLUDE})
add_executable(./bin/controls controls.cpp ${SOURCES} ${INCLUDE})
add_executable(./bin/ecs ecs.cpp ${SOURCES} ${INCLUDE})
add_executable(./bin/rendering rendering.cpp ${SOURCES} ${INCLUDE})
add_executable(./bin/scenes scenes.cpp ${SOURCES} ${INCLUDE})
add_executable(./bin/initialize initialize.cpp ${SOURCES} ${INCLUDE})