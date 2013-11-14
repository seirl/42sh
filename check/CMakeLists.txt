file(COPY . DESTINATION . FILES_MATCHING PATTERN "*.test")
file(COPY . DESTINATION . FILES_MATCHING PATTERN "test_*.py")
file(COPY qdunittest run_tests.py DESTINATION .)

add_executable(test_lexer
    test_lexer.c
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:hashtbl>
)

add_executable(run_readline_test
    test_readline.c
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:readline>
)
