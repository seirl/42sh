#include <string.h>
#include "string_utils.h"
#include "lexer_private.h"

static s_string *heredoc_delim(s_lexer *lexer)
{
    char c = 0;
    char prev = 0;
    s_string *delim = string_create(0);
    while (1)
    {
        c = lexer->topc(lexer);
        if (c == 0 || ((prev != '\\') && (c == '&' || c == '|' || c == ' ')))
            break;
        string_putc(delim, lexer->getc(lexer));
        prev = c;
    }
    return delim;
}

static int cmp_str_delim(s_string *body, s_string *delim)
{
    s_string *end = string_extract(body, body->len - delim->len, 0);
    int ret = strcmp(end->buf, delim->buf);
    string_free(end);
    return ret;
}

static void heredoc_body(s_lexer *lexer, s_string *delim)
{
    char c = 0;
    char prev = 0;
    s_string *body = string_create(0);
    while (1)
    {
        c = lexer->topc(lexer);
        if (c == 0)
            break;
        string_putc(body, lexer->getc(lexer));
        if (cmp_str_delim(body, delim) == 0)
        {
            lexer->working_buffer = string_extract(body, 0,
                    body->len - delim->len - 1);
            string_free(body);
            return;
        }
        prev = c;
    }
}

int lex_heredoc(s_lexer *lexer)
{
    if (lexer->prev_heredoc == 0)
        return 0;
    
    s_string *delim = heredoc_delim(lexer);
    lex_eat_spaces(lexer);
    heredoc_body(lexer, delim);
    lexer->token_type = T_WORD;
    string_free(delim);
    lexer->prev_heredoc = 0;
    return 1;
}
