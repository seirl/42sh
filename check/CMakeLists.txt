file(COPY . DESTINATION . FILES_MATCHING PATTERN "*.test")
file(COPY qdunittest run_tests.py DESTINATION .)

add_executable(test_lexer
    test_lexer.c
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:smalloc>
    $<TARGET_OBJECTS:hashtbl>
    $<TARGET_OBJECTS:readline>
)
