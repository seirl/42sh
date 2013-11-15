#include "lexer_private.h"

#include "smalloc.h"

s_lexer *lex_create(char (*lex_getc)(void *input_state),
                    char (*lex_topc)(void *input_state),
                    char *source)
{
    s_lexer *lexer;

    lexer = smalloc(sizeof (s_lexer));

    lexer->getc = lex_getc;
    lexer->topc = lex_topc;
    lexer->source = source;
    lexer->working_buffer = string_create(0);
    lexer->token_type = T_WORD;
    lexer->sur.begin = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
    lexer->quoted = 0;
    lexer->concat = 0;
    lexer->lookahead = NULL;

    return lexer;
}
