cmake_minimum_required(VERSION 2.6)
project(42sh)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -std=c99")
include_directories(src)
include_directories(src/utils)
include_directories(src/lexer)
set(cfiles
    src/main.c

    src/utils/smalloc.c
    src/utils/string_add.c
    src/utils/string_create.c
    src/utils/string_get.c
    src/utils/string_replace.c

    src/lexer/lexer.c
    src/lexer/token.c
    src/lexer/token_queue.c
)
add_executable(42sh ${cfiles})
