cmake_minimum_required(VERSION 3.16)
project(all_contests)

set(CMAKE_CXX_STANDARD 17)

# Можно добавить -std=c++17 -O2.
set(CMAKE_CXX_FLAGS_ASAN "-g -Wall -Werror -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

# Включить нужный контест.
add_executable(all_contests contest_algo_18/D.cpp)
