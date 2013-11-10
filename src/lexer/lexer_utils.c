#include "lexer_private.h"

char lex_topc(s_lexer *lexer)
{
    return lexer->topc(lexer->input_state);
}

char lex_getc(s_lexer *lexer)
{
    return lexer->getc(lexer->input_state);
}

void lex_discard(s_lexer *lexer)
{
    lexer->getc(lexer->input_state);
}

void lex_eat(s_lexer *lexer)
{
    string_putc(lexer->working_buffer, lex_getc(lexer));
    location_next_column(&(lexer->location));
}
