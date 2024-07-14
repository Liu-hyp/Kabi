cmake_minimum_required(VERSION 3.17)
project(Kabi)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/net ${PROJECT_SOURCE_DIR}/net/tcp ${PROJECT_SOURCE_DIR}/net/coder)
add_library(${PROJECT_NAME}_core src/config.cpp src/log.cpp src/util.cpp net/eventloop.cpp net/fd_event.cpp net/wakeup_fd_event.cpp net/timer_event.cpp net/timer.cpp net/io_thread.cpp net/io_thread_group.cpp
net/tcp/tcpbuffer.cpp net/tcp/net_addr.cpp net/tcp/tcp_acceptor.cpp net/tcp/tcp_server.cpp net/tcp/tcp_connection.cpp net/fd_event_group.cpp net/tcp/tcp_client.cpp
net/coder/tinypb_coder.cpp net/coder/tinypb_protocol.cpp)
link_directories((${PROJECT_SOURCE_DIR}/lib))
target_link_libraries(${PROJECT_NAME}_core libtinyxml.a)
add_executable(${PROJECT_NAME} test_client.cpp)
#add_executable(${PROJECT_NAME} test_tcp.cpp)
#链接操作
target_link_libraries(${PROJECT_NAME} Kabi_core)

