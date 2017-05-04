cmake_minimum_required(VERSION 3.6)
project(Heptapod)

set(CMAKE_CXX_STANDARD 14)

# Common definitions
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_SEPARATE_COMPILATION)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(-D__USE_W32_SOCKETS)
add_definitions(-D_WIN32_WINNT=0x0a00)

# Build options
option(BUILD_EXAMPLES "Build the examples" ON)

# Common include dirs and libs
include_directories(SYSTEM dependencies/include)
include_directories(include)

link_libraries(ws2_32)
link_libraries(wsock32)

add_library(asio dependencies/src/asio/asio.cpp)
add_library(fmt dependencies/src/fmt/format.cc dependencies/src/fmt/ostream.cc dependencies/src/fmt/posix.cc)
add_library(hpt
        src/io_service_pool.cc
        src/io_thread_group.cc
        src/rpc_server.cc
        src/rpc_client.cc
        src/rpc_listener.cc
        src/rpc_stream.cc
        src/rpc_server_stream.cc
        src/enable_wait_signal.cc
        )

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif (BUILD_EXAMPLES)
