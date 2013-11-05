cmake_minimum_required(VERSION 2.6)
project(42sh)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c99")
add_subdirectory(src)
