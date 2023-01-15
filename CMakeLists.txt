cmake_minimum_required(VERSION 3.16)
project(ft_ssl C)

set(CMAKE_C_STANDARD 11)

add_executable(ft_ssl
        ft_ssl.c print.c ft_ssl.h simple_cb_parser.c src/argument_parser/argument_parser.h src/argument_parser/str_match.c src/argument_parser/argument_parser.c src/main/ft_ssl.c src/digests/digests.h src/digests/common_arguments.c src/main/parser.c src/main/ft_ssl.h src/digests/md5.c src/digests/sha256.c src/main/read_file.c src/digests/common_functions.c src/main/compact_array.c src/main/print_utils.c src/main/print_utils.h src/main/compact_array.h src/digests/common_functions.h)
add_custom_command(TARGET ft_ssl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ft_ssl> ..)
