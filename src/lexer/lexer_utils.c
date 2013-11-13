#include <string.h>
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

static int is_valid_operator(s_string *s)
{
#define X(Tok, Str)            \
    if (!strcmp(s->buf, Str))  \
        return 1;
#include "operator.def"
#undef X
    return 0;
}

int lex_fill_delimiter(s_lexer *lexer)
{
    char c;
    do {
        c = lexer->topc(lexer);
        string_putc(lexer->working_buffer, c);
        if (!is_valid_operator(lexer->working_buffer))
        {
            string_del_from_end(lexer->working_buffer, 1);
            return 0;
        }
        lexer->getc(lexer);
    } while (1);
}

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
        {
            if (prev == 0)
            {
                if (c == '\n')
                    string_putc(lexer->working_buffer, lexer->getc(lexer));
                else
                    lex_fill_delimiter(lexer);
            }
            break;
        }
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return c == 0 || c == EOF;
}
