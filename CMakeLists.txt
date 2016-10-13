cmake_minimum_required(VERSION 2.8)
project(blueprint)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -Werror")

set(SOURCE_FILES
        src/main.cpp
        src/dictionary.h
        src/dictionary.cpp
        src/template.h
        src/template.cpp
        src/default_dictionary.h
        src/default_dictionary.cpp
        src/lookup.h
        src/lookup.cpp)
add_executable(blueprint ${SOURCE_FILES})