cmake_minimum_required(VERSION 3.17)
project(Kabi)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/kabisrc/include ${PROJECT_SOURCE_DIR}/kabisrc/net ${PROJECT_SOURCE_DIR}/kabisrc/net/tcp ${PROJECT_SOURCE_DIR}/kabisrc/net/coder)

add_library(${PROJECT_NAME}_core kabisrc/include/config.cpp kabisrc/include/log.cpp kabisrc/include/util.cpp kabisrc/net/eventloop.cpp kabisrc/net/fd_event.cpp kabisrc/net/wakeup_fd_event.cpp 
    kabisrc/net/timer_event.cpp kabisrc/net/timer.cpp kabisrc/net/io_thread.cpp kabisrc/net/io_thread_group.cpp
    kabisrc/net/tcp/tcpbuffer.cpp kabisrc/net/tcp/net_addr.cpp kabisrc/net/tcp/tcp_acceptor.cpp kabisrc/net/tcp/tcp_server.cpp kabisrc/net/tcp/tcp_connection.cpp kabisrc/net/fd_event_group.cpp kabisrc/net/tcp/tcp_client.cpp
    kabisrc/net/coder/tinypb_coder.cpp kabisrc/net/coder/tinypb_protocol.cpp
    kabisrc/net/rpc/rpc_controller.cpp kabisrc/net/rpc/rpc_dispatcher.cpp
    testcases/order.pb.cc kabisrc/include/msg_util.cpp kabisrc/net/rpc/rpc_channel.cpp kabisrc/include/runtime.cpp
    kabisrc/net/game/AOI.cpp kabisrc/net/game/msg_api.cpp message.pb.cc kabisrc/net/game/game_role.cpp kabisrc/net/game/GameMsg.pb.cc)

link_directories((${PROJECT_SOURCE_DIR}/kabisrc/lib))
target_link_libraries(${PROJECT_NAME}_core libtinyxml.a)
#add_executable(${PROJECT_NAME} test_client.cpp)
#add_executable(${PROJECT_NAME} test_tcp.cpp)
#add_executable(${PROJECT_NAME} testcases/test_rpc_client.cpp)
#add_executable(${PROJECT_NAME} testcases/test_rpc_server.cpp)
add_executable(${PROJECT_NAME} testcases/test_game_client.cpp)
#add_executable(${PROJECT_NAME} testcases/test_game_server.cpp)
#链接操作
target_link_libraries(${PROJECT_NAME} Kabi_core ${Protobuf_LIBRARY})
#target_link_libraries(gameserver Kabi_core ${Protobuf_LIBRARY})
#target_link_libraries(gameclient Kabi_core ${Protobuf_LIBRARY})

