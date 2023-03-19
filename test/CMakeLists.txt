cmake_minimum_required(VERSION 3.24)
project(test_wordchain)

set(CMAKE_CXX_STANDARD 14)

# 增加一个子目录准备使用gtest自带的cmakelists.txt去编译gtest
add_subdirectory(lib)
include_directories(lib/googletest/include)
include_directories(lib/googlemock/include)

add_executable(UnitTest1 test_core.cpp src/core.cpp)
add_executable(UnitTest2 test_main.cpp src/main.cpp src/core.cpp)


target_link_libraries(UnitTest1 gtest gtest_main)
target_link_libraries(UnitTest1 gmock gmock_main)
target_link_libraries(UnitTest2 gtest gtest_main)
target_link_libraries(UnitTest2 gmock gmock_main)