#include <ctype.h>
#include "lexer_private.h"

int handle_operator(s_lexer *lexer)
{
    char c;
    do {
        c = lexer->topc(lexer->input_state);
        string_putc(lexer->working_buffer, c);
        if (!is_valid_operator(lexer, lexer->working_buffer) || c == 0)
        {
            string_del_from_end(lexer->working_buffer, 1);
            return 1;
        }
        lexer->getc(lexer->input_state);
    } while (c != 0);
    return 0;
}

int handle_quotes(s_lexer *lexer, char c, char prev)
{
    if (prev == '\\')
        return 0;
    if (c == '\'' || c == '\"' || c == '`')
    {
        lexer->sur.end = c;
        lexer->sur.count = 1;
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
        return fill_until(lexer, 1);
    }
    return 0;
}

int handle_dollar(s_lexer *lexer, char c, char prev)
{
    if (prev != 0)
        return 0;
    if (c == '$')
    {
        string_putc(lexer->working_buffer, lexer->getc(lexer->input_state));
        c = lexer->topc(lexer->input_state);
        if (c == '(' || c == '{')
        {
            lexer->sur.begin = c;
            lexer->sur.end = (c == '(') ? ')' : '}';
            lexer->sur.count = 1;
            fill_until(lexer, 1);
        }
        else
            fill_upto_delim(lexer);
        return 1;
    }
    return 0;
}

int handle_comment(s_lexer *lexer, char c, char prev)
{
    if (prev != 0)
        return 0;
    if (c == '#')
    {
        lexer->sur.begin = 0;
        lexer->sur.end = '\n';
        lexer->sur.count = 1;
        fill_until(lexer, 1);
        lexer->token_type = T_NEWLINE;
        lexer->concat = -1;
        return 1;
    }
    return 0;
}

static int is_in_name_charset(char c)
{
    return isdigit(c) || isalpha(c) || c == '_';
}

//0 -> no =, 1 -> assign, 2 -> word
int handle_assignment(s_lexer *lexer, char c)
{
    s_string *wbuf = lexer->working_buffer;
    if (c == '=')
    {
        if (wbuf->len == 0)
        {
            string_putc(wbuf, lexer->getc(lexer->input_state));
            return 1;
        }
        for (size_t i = 0; i < wbuf->len; ++i)
        {
            if (i == 0 && isdigit(wbuf->buf[i]))
                return 1;
            if (!is_in_name_charset(wbuf->buf[i]))
                return 1;
        }
        lexer->token_type = T_ASSIGNMENT_WORD;
        return 1;
    }
    return 0;
}
