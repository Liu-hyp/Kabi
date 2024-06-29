cmake_minimum_required(VERSION 3.17)
project(Kabi)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(PROJECT_KABI_DIR, ${PROJECT_SOURCE_DIR}/kabi)
set(PROJECT_H_DIR, ${PROJECT_KABI_DIR}/commonc)

#编译子文件夹的CMakeLists.txt
add_subdirectory(kabisrc)

#生成可执行文件
add_executable(${PROJECT_NAME} test_log.cpp)
#链接操作
target_link_libraries(${PROJECT_NAME} common)

