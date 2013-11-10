cmake_minimum_required(VERSION 2.6)
project(42sh)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -Werror -pedantic -g3")

include_directories(src)
include_directories(src/utils)
include_directories(src/getopt)
include_directories(src/lexer)

add_library(smalloc OBJECT
    src/utils/smalloc.c
)

add_library(string OBJECT
    src/utils/string_add.c
    src/utils/string_cmp.c
    src/utils/string_create.c
    src/utils/string_get.c
    src/utils/string_replace.c
    src/utils/string_update.c
)

add_library(lexer OBJECT
    src/lexer/lexer.c
    src/lexer/lexer_quoting.c
    src/lexer/lexer_release.c
    src/lexer/lexer_utils.c
    src/lexer/token.c
    src/lexer/location.c
)

add_library(getopt OBJECT
    src/getopt/shopt.c
)

set(cfiles
    src/main.c
    src/utils/log.c
    src/sighandler.c
)

add_executable(42sh
    ${cfiles}
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:getopt>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:smalloc>
)

MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_CPP_FLAGS: " ${CMAKE_CPP_FLAGS} )

# Tests and checks
add_subdirectory(check)
