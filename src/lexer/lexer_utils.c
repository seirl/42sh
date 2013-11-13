#include "char_utils.h"
#include "lexer_private.h"

#define EOF -1

static void lex_eat_spaces(s_lexer *lexer)
{
    char c;
    while ((c = lexer->topc(lexer)) == ' ')
        lexer->getc(lexer);
}

#include <stdio.h>
#undef getc
int lex_fill_buf(s_lexer *lexer, int eat_spaces)
{
    string_reset(lexer->working_buffer);
    char prev = 0;
    char c;
    if (eat_spaces)
        lex_eat_spaces(lexer);
    int quote = 0;
    do
    {
        c = lexer->topc(lexer);
        int current_quote = is_quote(c) * (prev != '\\');
        quote ^= current_quote;
        if (is_delimiter(c) && prev != '\\' && quote == 0)
            break;
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return c == 0 || c == EOF;
}
