cmake_minimum_required(VERSION 3.17)
project(Kabi)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/net)
add_library(${PROJECT_NAME}_core src/config.cpp src/log.cpp src/util.cpp net/eventloop.cpp net/fd_event.cpp net/wakeup_fd_event.cpp)
link_directories((${PROJECT_SOURCE_DIR}/lib))
target_link_libraries(${PROJECT_NAME}_core libtinyxml.a)
add_executable(${PROJECT_NAME} test_eventloop.cpp)
#链接操作
target_link_libraries(${PROJECT_NAME} Kabi_core)

