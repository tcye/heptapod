cmake_minimum_required(VERSION 3.6)
project(Heptapod)

set(CMAKE_CXX_STANDARD 14)

# Common definitions
add_definitions(-DASIO_STANDALONE)
add_definitions(-D__USE_W32_SOCKETS)
add_definitions(-D_WIN32_WINNT=0x0a00)

# Build options
option(BUILD_EXAMPLES "Build the examples" ON)

# Common include dirs and libs
include_directories(SYSTEM dependencies/include)
include_directories(include)

link_libraries(ws2_32)
link_libraries(wsock32)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif (BUILD_EXAMPLES)
