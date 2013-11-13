cmake_minimum_required(VERSION 2.6)
project(42sh)

add_definitions(-DCONCHITA)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -Werror -pedantic -g3 -gdwarf-2")

include_directories(src)
include_directories(src/utils)
include_directories(src/options)
include_directories(src/lexer)
include_directories(src/readline)

add_library(smalloc OBJECT
    src/utils/smalloc.c
)

add_library(hashtbl OBJECT
    src/utils/hashtbl.c
)

add_library(string OBJECT
    src/utils/string_add.c
    src/utils/string_cmp.c
    src/utils/string_create.c
    src/utils/string_read.c
    src/utils/string_replace.c
    src/utils/string_update.c
)

add_library(lexer OBJECT
    src/lexer/lexer.c
    src/lexer/lexer_create.c
    src/lexer/lexer_delete.c
    src/lexer/lexer_release.c
    src/lexer/lexer_utils.c
    src/lexer/char_utils.c
    src/lexer/token.c
    src/lexer/location.c
    src/lexer/alias.c
)

add_library(options OBJECT
    src/options/options.c
    src/options/getopt.c
    src/options/shopt.c
)

add_library(readline OBJECT
    src/readline/readline.c
    src/readline/characters.c
)

set(cfiles
    src/main.c
    src/utils/log.c
    src/sighandler.c
)

add_executable(42sh
    ${cfiles}
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:options>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:readline>
)

MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_CPP_FLAGS: " ${CMAKE_CPP_FLAGS} )

# Tests and checks
add_subdirectory(check)
