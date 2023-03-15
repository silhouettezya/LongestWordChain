cmake_minimum_required(VERSION 3.24)
project(Wordlist)

set(CMAKE_CXX_STANDARD 14)


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH   ${PROJECT_SOURCE_DIR}/bin/)


add_library(core SHARED src/core.cpp src/core.h)
set_target_properties(core PROPERTIES PREFIX "")

add_executable(Wordlist src/main.cpp)
target_link_libraries(Wordlist core)
