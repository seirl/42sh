file(COPY . DESTINATION . FILES_MATCHING PATTERN "*.sh")
file(COPY . DESTINATION . FILES_MATCHING PATTERN "*.test")
file(COPY . DESTINATION . FILES_MATCHING PATTERN "test_*.py")
file(COPY qdunittest run_tests.py DESTINATION .)


add_executable(test_fnmatch test_fnmatch.c $<TARGET_OBJECTS:fnmatch>)

add_executable(test_interface test_interface.c $<TARGET_OBJECTS:options>)
target_link_libraries(test_interface lib42sh)

add_executable(test_ast_printer test_ast_printer.c)
target_link_libraries(test_ast_printer lib42sh)

add_executable(test_env test_env.c)
target_link_libraries(test_env lib42sh)

add_executable(test_exec test_exec.c)
target_link_libraries(test_exec lib42sh)

add_executable(test_lexer test_lexer.c)
target_link_libraries(test_lexer lib42sh)

add_executable(test_parser test_parser.c)
target_link_libraries(test_parser lib42sh)

add_executable(test_readline test_readline.c)
target_link_libraries(test_readline lib42sh)

add_executable(test_utils test_utils.c)
target_link_libraries(test_utils lib42sh)
