#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "lexer_private.h"
#include "input_private.h"

#undef getc

static void update_quote(s_lexer *lexer, char c, char prev)
{
    if ((c == '\'' || c == '\"')
       && prev != '\\'
       && lexer->sur.end != '\''
       && lexer->sur.end != '\"')
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
    while ((c = lex_topc(lexer)) == ' ' || c == '\t')
    {
        ret = 1;
        lex_getc(lexer);
    }
    return ret;
}

static void lexer_until_reset(s_lexer *lexer)
{
    lexer->quoted = 0;
    lexer->sur.end = 0;
    lexer->sur.count = 0;
}

//0-> nothing, 1->break, -1->continue
static int check_close(s_lexer *lexer, char c, char prev)
{
    if (lexer->quoted == 0 && (lexer->sur.end == '\'' || prev != '\\')
       && (c == lexer->sur.end || c == '\0'))
    {
        if (c == 0 && lexer->sur.end != '\n')
        {
            if (lexer->input->next(lexer->input, "PS2") == 0)
            {
                lex_error_set(lexer, E_LEX_UNBALANCED);
                return 1;
            }
            return -1;
        }
        else
        {
            lexer->sur.count -= 1;
            if (lexer->sur.count == 0)
                return 1;
        }
    }
    return 0;
}

static int check_open(s_lexer *lexer, char c, char prev)
{
    if (prev && lexer->quoted == 0
            && (c == lexer->sur.begin || c == 0))
    {
        if (c == 0 && lexer->sur.end != '\n')
        {
            if (lexer->input->next(lexer->input, "PS2") == 0)
            {
                lex_error_set(lexer, E_LEX_UNBALANCED);
                return 1;
            }
            return -1;
        }
        else
            lexer->sur.count += 1;
    }
    return 0;
}

int fill_until(s_lexer *lexer, int include_last)
{
    char c;
    char prev = 0;
    int ret;
    while (1)
    {
        c = lex_topc(lexer);
        ret = check_close(lexer, c, prev);
        if (ret == 1)
            break;
        if (ret == -1)
            continue;
        ret = check_open(lexer, c, prev);
        if (ret == -1)
            continue;
        string_putc(lexer->working_buffer, lex_getc(lexer));
        update_quote(lexer, c, prev);
        prev = c;
    }
    if (include_last && c != 0)
        string_putc(lexer->working_buffer, lex_getc(lexer));
    lexer_until_reset(lexer);
    return c != 0;
}

int fill_upto_delim(s_lexer *lexer, char *custom_delim)
{
    char c;
    char prev = 0;
    do {
        c = lex_topc(lexer);
        if ((is_quote(c)
            || is_delimiter(lexer, c)
            || strchr(custom_delim, c)
            || c == ' '
            || c == '\t'
            || c == '\n')
            && prev != '\\')
            break;
        string_putc(lexer->working_buffer, lex_getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return 1;
}

static int handle_delim_and_quote(s_lexer *lexer, char c, char prev)
{
    if ((is_delimiter(lexer, c)
        || (lexer->context == LEX_ALL && is_quote(c))
        || c == '$')
       && prev != '\\')
    {
        if (prev == 0)
        {
            if (handle_quotes(lexer, c, prev))
                return 1;
            if (c == '\n')
                string_putc(lexer->working_buffer,
                           lex_getc(lexer));
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
    int spaces = 0;

    if (lexer->context == LEX_ALL)
        spaces = lex_eat_spaces(lexer);
    lexer->concat = lexer->concat == -1 ? 0 : !spaces;

    do {
        c = lex_topc(lexer);
        if (lexer->context == LEX_ALL && lexer->assignment == 0
            && handle_comment(lexer, c, prev))
            break;
        if (handle_dollar(lexer, c, prev))
            break;
        if (handle_assignment(lexer, c))
            break;
        if (handle_delim_and_quote(lexer, c, prev))
            break;
        string_putc(lexer->working_buffer, lex_getc(lexer));
        prev = c;
    } while (c != 0 && c != EOF);
    return 0;
}
