cmake_minimum_required(VERSION 2.6)
project(42sh)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

if(EXISTS "${CMAKE_MODULE_PATH}/common.cmake")
    include(common)
else()
    message(FATAL_ERROR "Run ./configure")
endif()

if(COVERAGE)
    include(tools/CodeCoverage)
    setup_target_for_coverage(coverage
                              ./tests/run_tests.py
                              coverage
    )
endif()

add_definitions(-D_XOPEN_SOURCE)
LINK_LIBRARIES(m ncurses)

include_directories(src)
include_directories(src/env)
include_directories(src/ast)
include_directories(src/lexer)
include_directories(src/fnmatch)
include_directories(src/options)
include_directories(src/parser)
include_directories(src/readline)
include_directories(src/utils)
include_directories(src/exec)

add_library(smalloc OBJECT
    src/utils/smalloc.c
)

add_library(hashtbl OBJECT
    src/utils/hashtbl.c
)

add_library(log OBJECT
    src/utils/log.c
)

add_library(env OBJECT
    src/env/env.c
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
    src/fnmatch/fnmatch.c
)

add_library(lexer OBJECT
    src/lexer/lexer.c
    src/lexer/lexer_create.c
    src/lexer/lexer_delete.c
    src/lexer/lexer_delim.c
    src/lexer/lexer_heredoc.c
    src/lexer/lexer_lookahead.c
    src/lexer/lexer_release.c
    src/lexer/char_utils.c
    src/lexer/token.c
    src/lexer/token_debug.c
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
    src/readline/key.c
    src/readline/wrapper.c
    src/readline/bracket_key.c
    src/readline/special_keys.c
    src/readline/terminal.c
)

add_library(parser OBJECT
    src/parser/parser_create.c
    src/parser/parser_delete.c

    src/parser/parser_utils.c
    src/parser/parse_expect.c

    src/parser/parse_rule_and_or.c
    src/parser/parse_rule_command.c
    src/parser/parse_rule_compound_list.c
    src/parser/parse_rule_do_group.c
    src/parser/parse_rule_elements.c
    src/parser/parse_rule_for.c
    src/parser/parse_rule_funcdec.c
    src/parser/parse_rule_input.c
    src/parser/parse_rule_list.c
    src/parser/parse_rule_pipeline.c
    src/parser/parse_rule_prefix.c
    src/parser/parse_rule_redirection.c
    src/parser/parse_rule_shell_command.c
    src/parser/parse_rule_simple_command.c

    src/parser/parse_assignment.c
    src/parser/parse_compound_words.c
    src/parser/parse_heredoc.c
)

add_library(exec OBJECT
    src/exec/exec_bang.c
    src/exec/exec.c
    src/exec/exec_cmds.c
    src/exec/exec_control.c
    src/exec/exec_func.c
    src/exec/exec_or_and.c
    src/exec/exec_pipes.c
    src/exec/exec_redir.c
)

add_library(ast OBJECT
    src/ast/compound_word.c
    src/ast/word.c
)

add_executable(../42sh
    src/main.c
    src/sighandler.c

    $<TARGET_OBJECTS:ast>
    $<TARGET_OBJECTS:env>
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:options>
    $<TARGET_OBJECTS:readline>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:string>
)

message(STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS})
message(STATUS "CMAKE_CPP_FLAGS: " ${CMAKE_CPP_FLAGS})

# Tests and checks
add_subdirectory(tests)
