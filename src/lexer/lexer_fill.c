#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "lexer_private.h"

#undef getc

static void update_quote(s_lexer *lexer, char c, char prev)
{
    if ((c == '\'' || c == '\"') && prev != '\\')
    {
        if (lexer->quoted == c)
            lexer->quoted = 0;
        else if (lexer->quoted == 0)
            lexer->quoted = c;
    }
}

int lex_eat_spaces(s_lexer *lexer)
{
    char c;
    int ret = 0;
    while ((c = lexer->topc(lexer->input_state)) == ' ' || c == '\t')
    {
        ret = 1;
        lexer->getc(lexer->input_state);
    }
    return ret;
}

int fill_until(s_lexer *lexer, int include_last)
{
    char c;
    char prev = 0;
    while (1)
    {
        c = lexer->topc(lexer->input_state);
        if (lexer->quoted == 0 && prev != '\\'
           && (c == lexer->sur.end || c == '\0'))
        {
            lexer->sur.count -= 1;
            if (lexer->sur.count == 0)
                break;
        }
        if (prev && lexer->quoted == 0 && (c == lexer->sur.begin || c == '\0'))
        {
            lexer->sur.count += 1;
            if (lexer->sur.count == 0)
            {
                ;//syntax error
            }
        }
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
        update_quote(lexer, c, prev);
        prev = c;
    }
    if (include_last && c != 0)
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
    lexer->quoted = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
    return c != 0;
}

int fill_upto_delim(s_lexer *lexer)
{
    char c;
    char prev = 0;
    do {
        c = lexer->topc(lexer->input_state);
        if ((is_quote(c) || is_delimiter(c)) && prev != '\\')
            break;
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
        prev = c;
    } while (c != 0 && c != EOF);
    return 1;
}

static int handle_delim_and_quote(s_lexer *lexer, char c, char prev)
{
    if ((is_delimiter(c) || is_quote(c) || c == '$') && prev != '\\')
    {
        if (prev == 0)
        {
            if (handle_quotes(lexer, c, prev))
                return 1;
            if (c == '\n')
                string_putc(lexer->working_buffer,
                           lexer->getc(lexer->input_state));
            else
                handle_operator(lexer);
        }
        return 1;
    }
    return 0;
}

int fill_token(s_lexer *lexer)
{
    char c;
    char prev = 0;

    int spaces = lex_eat_spaces(lexer);
    lexer->concat = lexer->concat == -1 ? 0 : !spaces;

    do {
        c = lexer->topc(lexer->input_state);
        if (handle_comment(lexer, c, prev))
            break;
        if (handle_dollar(lexer, c, prev))
            break;
        if (handle_assignment(lexer, c))
            break;
        if (handle_delim_and_quote(lexer, c, prev))
            break;
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
        prev = c;
    } while (c != 0 && c != EOF);
    return 0;
}
