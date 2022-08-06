cmake_minimum_required(VERSION 3.10)

set(CC g++)
set(CMAKE_CXX_STANDARD 14)
project(base64)

file(GLOB all_srcs ${PROJECT_SOURCE_DIR}/src/*)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Wshadow")
add_executable(base64 ${all_srcs})
