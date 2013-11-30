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

add_definitions(-D_GNU_SOURCE)
add_definitions(-D_XOPEN_SOURCE)
add_definitions(-D_XOPEN_SOURCE_EXTENDED)

link_libraries(m ncurses)

include_directories(src)
include_directories(src/ast)
include_directories(src/ast_printer)
include_directories(src/builtins)
include_directories(src/functions)
include_directories(src/env)
include_directories(src/evalexpr)
include_directories(src/exec)
include_directories(src/expand)
include_directories(src/match)
include_directories(src/history)
include_directories(src/input)
include_directories(src/lexer)
include_directories(src/options)
include_directories(src/parser)
include_directories(src/readline)
include_directories(src/shell)
include_directories(src/utils)
include_directories(src/autocompletion)

add_library(smalloc OBJECT
    src/utils/smalloc.c
)

add_library(hashtbl OBJECT
    src/utils/hashtbl.c
    src/utils/hash_op.c
    src/utils/hashtbl_dup.c
    src/utils/hash_char.c
    src/utils/hash_string.c
)

add_library(log OBJECT
    src/utils/log.c
)

add_library(env OBJECT
    src/env/env.c
    src/env/env_duplicate.c
    src/env/env_start.c
    src/env/env_special.c
    src/env/env_type.c
)

add_library(string OBJECT
    src/utils/string_add.c
    src/utils/string_cmp.c
    src/utils/string_create.c
    src/utils/string_delete.c
    src/utils/string_numbers.c
    src/utils/string_read.c
    src/utils/string_replace.c
    src/utils/string_update.c
    src/utils/string_search.c
)

add_library(match OBJECT
    src/match/fnmatch.c
    src/match/glob.c
    src/match/extmatch.c
)

add_library(lexer OBJECT
    src/lexer/alias.c
    src/lexer/alias_print.c
    src/lexer/alias_clear.c
    src/lexer/char_utils.c
    src/lexer/lexer_backslash.c
    src/lexer/lexer_error.c

    src/lexer/lexer.c
    src/lexer/lexer_fill.c
    src/lexer/lexer_heredoc.c
    src/lexer/lexer_input.c
    src/lexer/lexer_lookahead.c
    src/lexer/lexer_pattern.c
    src/lexer/lexer_struct.c
    src/lexer/lexer_word.c
    src/lexer/lexer_alias.c
    src/lexer/lexer_strip.c

    src/lexer/location.c
    src/lexer/token.c
    src/lexer/token_debug.c
    src/lexer/token_utils.c
    src/lexer/location.c
)

add_library(options OBJECT
    src/options/options.c
    src/options/getopt.c
    src/options/getopt_struct.c
    src/options/shopt.c
)

add_library(builtins OBJECT
    src/builtins/builtins.c

    src/builtins/b_alias.c
    src/builtins/b_break.c
    src/builtins/b_cd.c
    src/builtins/b_continue.c
    src/builtins/b_echo.c
    src/builtins/b_exec.c
    src/builtins/b_exit.c
    src/builtins/b_set.c
    src/builtins/b_shopt.c
    src/builtins/b_source.c
    src/builtins/b_unalias.c
    src/builtins/b_unset.c
    src/builtins/b_history.c
)

add_library(readline OBJECT
    src/readline/readline.c
    src/readline/key.c
    src/readline/wrapper.c
    src/readline/bracket_key.c
    src/readline/special_keys.c
    src/readline/special_line_manipulation_keys.c
    src/readline/escape_keys.c
    src/readline/terminal.c
    src/readline/prompt.c
    src/readline/konami.c
)

add_library(history OBJECT
    src/history/history.c
    src/history/hist_list.c
    src/history/history_observators.c
)

add_library(autocompletion OBJECT
    src/autocompletion/autocompletion.c
)

add_library(input OBJECT
    src/input/input.c

    src/input/input_new.c
    src/input/input_delete.c
    src/input/input_destroy.c

    src/input/input_string.c
    src/input/input_file.c
    src/input/input_interactive.c
)

add_library(ast_printer OBJECT
    src/ast_printer/ast_printer.c
    src/ast_printer/cmds.c
    src/ast_printer/ctrl.c
    src/ast_printer/elements.c
    src/ast_printer/words.c
    src/ast_printer/ast_printer.h
)

add_library(parser OBJECT
    src/parser/parser_create.c
    src/parser/parser_delete.c

    src/parser/parse_expect.c
    src/parser/parser_diagnostic.c
    src/parser/parser_ready.c
    src/parser/parser_status.c
    src/parser/parser_utils.c

    src/parser/parse_rule_and_or.c
    src/parser/parse_rule_braces.c
    src/parser/parse_rule_case.c
    src/parser/parse_rule_case_clause.c
    src/parser/parse_rule_case_item.c
    src/parser/parse_rule_command.c
    src/parser/parse_rule_compound_list.c
    src/parser/parse_rule_do_group.c
    src/parser/parse_rule_elements.c
    src/parser/parse_rule_else.c
    src/parser/parse_rule_for.c
    src/parser/parse_rule_funcdec.c
    src/parser/parse_rule_if.c
    src/parser/parse_rule_input.c
    src/parser/parse_rule_list.c
    src/parser/parse_rule_pipeline.c
    src/parser/parse_rule_prefix.c
    src/parser/parse_rule_redirection.c
    src/parser/parse_rule_shell_command.c
    src/parser/parse_rule_simple_command.c
    src/parser/parse_rule_until.c
    src/parser/parse_rule_while.c

    src/parser/parse_assignment.c
    src/parser/parse_compound_words.c
    src/parser/parse_heredoc.c
)

add_library(exec OBJECT
    src/exec/exec.c
    src/exec/exec_cmds.c
    src/exec/exec_control.c
    src/exec/exec_elements.c
    src/exec/exec_or_and.c
    src/exec/exec_pipes.c
    src/exec/exec_program.c
    src/exec/exec_redir.c
    src/exec/exec_redir_utils.c
    src/exec/exec_utils.c
    src/exec/pipe_utils.c
    src/exec/redir_set.c
)

add_library(evalexpr OBJECT
    src/evalexpr/opstack.c
    src/evalexpr/numstack.c
    src/evalexpr/ops.c
    src/evalexpr/evalexpr.c
    src/evalexpr/shuntingyard.c
    src/evalexpr/eval/base.c
    src/evalexpr/eval/bit.c
    src/evalexpr/eval/pow.c
    src/evalexpr/eval/shift.c
    src/evalexpr/eval/unary.c
)

add_library(ast OBJECT

    src/ast/and_or.c
    src/ast/assignment.c
    src/ast/case.c
    src/ast/command.c
    src/ast/compound_word.c
    src/ast/element.c
    src/ast/else.c
    src/ast/for.c
    src/ast/funcdec.c
    src/ast/heredoc.c
    src/ast/if.c
    src/ast/input.c
    src/ast/io_number.c
    src/ast/list.c
    src/ast/pipeline.c
    src/ast/prefix.c
    src/ast/redirection.c
    src/ast/shell_command.c
    src/ast/simple_command.c
    src/ast/until.c
    src/ast/while.c
    src/ast/word.c
    src/ast/word_list.c
)

add_library(expand OBJECT
    src/expand/expand.c
    src/expand/expand_argv.c
    src/expand/expand_arithm.c
    src/expand/expand_simple_var.c
    src/expand/expand_simple_word.c
    src/expand/expand_substs_shell.c
    src/expand/expand_substs_var.c
    src/expand/expand_wordlist.c
    src/expand/expand_elements.c
    src/expand/expand_ast.c
    src/expand/expand_glob.c
    src/expand/ifs.c
    src/expand/expand_var.c
    src/expand/expand_squote.c
    src/expand/expand_dquote.c
    src/expand/backslash.c
)

add_library(shell OBJECT
    src/shell/shell.c
    src/shell/shell_eval_file.c
    src/shell/shell_eval_string.c
    src/shell/shell_loop.c
    src/shell/shell_status.c
    src/shell/res_ctrl.c
    src/functions/functions.c
)

add_library(lib42sh STATIC
    $<TARGET_OBJECTS:ast>
    $<TARGET_OBJECTS:ast_printer>
    $<TARGET_OBJECTS:builtins>
    $<TARGET_OBJECTS:env>
    $<TARGET_OBJECTS:evalexpr>
    $<TARGET_OBJECTS:expand>
    $<TARGET_OBJECTS:exec>
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:history>
    $<TARGET_OBJECTS:autocompletion>
    $<TARGET_OBJECTS:input>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:parser>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:match>
    $<TARGET_OBJECTS:options>
    $<TARGET_OBJECTS:readline>
    $<TARGET_OBJECTS:shell>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:string>
)

add_executable(../42sh
    src/main.c
    src/utils/sighandler.c
)

target_link_libraries(../42sh lib42sh)

message(STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS})
message(STATUS "CMAKE_CPP_FLAGS: " ${CMAKE_CPP_FLAGS})

# Tests and checks
add_subdirectory(tests)
