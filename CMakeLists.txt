cmake_minimum_required(VERSION 3.16)
project(ft_ssl C)

set(CMAKE_C_STANDARD 11)

include_directories(inc)

file(GLOB_RECURSE src CONFIGURE_DEPENDS "src/*.c")

add_executable(ft_ssl ${src} src/digests/block_reader.c inc/block_reader.h)

set_target_properties(ft_ssl PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ..)
