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

add_executable(test_parser
    test_parser.c
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:parser>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:string>
)

add_executable(test_fnmatch
    test_fnmatch.c
    $<TARGET_OBJECTS:fnmatch>
)

add_executable(run_readline_test
    test_readline.c
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:readline>
)

add_executable(test_exec
    test_exec.c
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:log>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:exec>
)
