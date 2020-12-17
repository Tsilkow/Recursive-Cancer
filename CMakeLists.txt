cmake_minimum_required(VERSION 3.10)

project(RecursiveCancer VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("inc")
set(SOURCES
	src/commons.cpp
	src/board.cpp)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_library(Modules SHARED ${SOURCES})

add_executable(RecursiveCancer main.cpp)
target_compile_options(RecursiveCancer PRIVATE -Wall)
target_link_libraries(RecursiveCancer sfml-graphics sfml-audio Modules)
