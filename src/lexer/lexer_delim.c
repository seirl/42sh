#include <string.h>
#include "char_utils.h"
#include "lexer_private.h"

#define EOF -1

int lex_eat_spaces(s_lexer *lexer)
{
    char c;
    int ret = 0;
    while ((c = lexer->topc(lexer)) == ' ')
    {
        ret = 1;
        lexer->getc(lexer);
    }
    return ret;
}

#include <stdio.h>
#undef getc

static int is_valid_operator(s_lexer *lexer, s_string *s)
{
#define X(Tok, Str)                     \
    if (!strcmp(s->buf, Str))           \
    {                                   \
        lexer->token_type = Tok;        \
        lexer->concat = -1;             \
        return 1;                       \
    }
#include "operator.def"
#undef X
    return 0;
}

static int handle_operator(s_lexer *lexer)
{
    char c;
    do {
        c = lexer->topc(lexer);
        string_putc(lexer->working_buffer, c);
        if (!is_valid_operator(lexer, lexer->working_buffer) || c == 0)
        {
            string_del_from_end(lexer->working_buffer, 1);
            return 1;
        }
        lexer->getc(lexer);
    } while (c != 0);
    return 0;
}

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

static int fill_until(s_lexer *lexer, int include_last)
{
    char c;
    char prev = 0;
    while (1)
    {
        c = lexer->topc(lexer);
        if (lexer->quoted == 0 && prev != '\\' && (c == lexer->sur.end || c == '\0'))
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
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        update_quote(lexer, c, prev);
        prev = c;
    }
    if (include_last && c != 0)
        string_putc(lexer->working_buffer, lexer->getc(lexer));
    lexer->quoted = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
    return c != 0;
}

static int fill_upto_delim(s_lexer *lexer)
{
    char c;
    char prev = 0;
    do
    {
        c = lexer->topc(lexer);
        if ((is_quote(c) || is_delimiter(c)) && prev != '\\')
            break;
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return 1;
}

static int handle_quotes(s_lexer *lexer, char c, char prev)
{
    if (prev == '\\')
        return 0;
    if (c == '\'' || c == '\"' || c == '`')
    {
        lexer->sur.end = c;
        lexer->sur.count = 1;
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        return fill_until(lexer, 1);
    }
    return 0;
}

static int handle_dollar(s_lexer *lexer, char c, char prev)
{
    if (prev != 0)
        return 0;
    if (c == '$')
    {
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        c = lexer->topc(lexer);
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

static int handle_comment(s_lexer *lexer, char c, char prev)
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

int handle_assignment(s_lexer *lexer, char c)
{
    if (c == '=')
    {
        for (size_t i = 0; i < lexer->working_buffer->len; ++i)
        {
            if (is_quote(lexer->working_buffer->buf[i]))
                return 0;
        }
        //if (fill_upto_delim(lexer) == 0)
        //    return 0;
        lexer->getc(lexer); //discard '='
        lexer->token_type = T_ASSIGNMENT_WORD;
        return 1;
    }
    return 0;
}

int lex_delimit_token(s_lexer *lexer)
{
    char c;
    char prev = 0;

    //if (lexer->sur.end)
    //    return fill_until(lexer, 0);
    int spaces = lex_eat_spaces(lexer);
    lexer->concat = lexer->concat == -1 ? 0 : !spaces;

    do
    {
        c = lexer->topc(lexer);
        if (handle_comment(lexer, c, prev))
            break;
        if (handle_dollar(lexer, c, prev))
            break;
        if (handle_assignment(lexer, c))
            break;
        if ((is_delimiter(c) || is_quote(c) || c == '$') && prev != '\\')
        {
            if (prev == 0)
            {
                if (handle_quotes(lexer, c, prev))
                    break;
                if (c == '\n')
                    string_putc(lexer->working_buffer, lexer->getc(lexer));
                else
                    handle_operator(lexer);
            }
            break;
        }
        string_putc(lexer->working_buffer, lexer->getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return 0;
}
