cmake_minimum_required(VERSION 3.6)
project(HptExamples)
set(CMAKE_CXX_STANDARD 14)

add_executable(EchoServer echo_server.cc)
target_link_libraries(EchoServer asio fmt hpt)

add_executable(EchoClient echo_client.cc)
target_link_libraries(EchoClient asio fmt hpt)

add_executable(Test test.cc)
target_link_libraries(Test asio fmt hpt)
