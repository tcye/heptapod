cmake_minimum_required(VERSION 3.6)
project(Heptapod)

set(CMAKE_CXX_STANDARD 14)

# Common definitions
add_definitions(-DASIO_STANDALONE)
add_definitions(-D__USE_W32_SOCKETS)

# Build options
option(BUILD_EXAMPLES "Build the examples" ON)

# Common include dirs and libs
include_directories(SYSTEM dependencies/include)
include_directories(include)

link_libraries(ws2_32)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif (BUILD_EXAMPLES)
