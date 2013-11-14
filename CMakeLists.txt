cmake_minimum_required(VERSION 2.6)
project(42sh)

add_definitions(-DCONCHITA)
add_definitions(-D_XOPEN_SOURCE)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -Werror -pedantic -g3 -gdwarf-2")

include_directories(src)
include_directories(src/ast)
include_directories(src/lexer)
include_directories(src/options)
include_directories(src/parser)
include_directories(src/readline)
include_directories(src/utils)

add_library(smalloc OBJECT
    src/utils/smalloc.c
)

add_library(hashtbl OBJECT
    src/utils/hashtbl.c
)

add_library(log OBJECT
    src/utils/log.c
)

add_library(string OBJECT
    src/utils/string_add.c
    src/utils/string_cmp.c
    src/utils/string_create.c
    src/utils/string_delete.c
    src/utils/string_read.c
    src/utils/string_replace.c
    src/utils/string_update.c
    src/utils/string_search.c
)

add_library(fnmatch OBJECT
    src/utils/my_fnmatch.c
)

add_library(lexer OBJECT
    src/lexer/lexer.c
    src/lexer/lexer_create.c
    src/lexer/lexer_delete.c
    src/lexer/lexer_release.c
    src/lexer/lexer_delim.c
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
    src/readline/terminal.c
)

add_library(parser OBJECT
    src/parser/parser_create.c
    src/parser/parser_delete.c
    src/parser/parse_rule_input.c
    src/parser/parse_rule_list.c
)

set(cfiles
    src/main.c
    src/sighandler.c
)

add_executable(../42sh
    ${cfiles}
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:options>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:readline>
)

MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_CPP_FLAGS: " ${CMAKE_CPP_FLAGS} )

# Tests and checks
add_subdirectory(check)
