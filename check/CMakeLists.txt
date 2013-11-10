add_executable(test_lexer
    test_lexer.c
    $<TARGET_OBJECTS:lexer>
    $<TARGET_OBJECTS:string>
    $<TARGET_OBJECTS:smalloc>
)
