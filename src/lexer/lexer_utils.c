#include <stdio.h>
#include "char_utils.h"
#include "lexer_private.h"

#undef getc
static void lex_eat_spaces(s_lexer *lexer)
{
    char c;
    while ((c = lexer->topc(lexer)) == ' ')
        lexer->getc(lexer);
}

int lex_fill_buf(s_lexer *lexer)
{
    char c;
    lex_eat_spaces(lexer);
    while (1)
    {
        c = lexer->topc(lexer);
        if (is_delimiter(c))
            break;
        string_putc(lexer->working_buffer, lexer->getc(lexer));
    }
    return c == 0 || c == EOF;
}
