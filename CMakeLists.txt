cmake_minimum_required(VERSION 3.10)

project(RecursiveCancer VERSION 1.0)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

include_directories("inc")
set(SOURCES
	src/commons.cpp
	src/growth.cpp
	src/board.cpp
	src/simulation.cpp)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_library(Modules SHARED ${SOURCES})

add_executable(RecursiveCancer main.cpp)
target_compile_options(RecursiveCancer PRIVATE -Wall)
target_link_libraries(RecursiveCancer sfml-graphics sfml-audio Modules)
